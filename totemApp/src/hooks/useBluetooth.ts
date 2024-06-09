import { useState, useEffect } from 'react';
import {
  NativeModules,
  NativeEventEmitter,
  Platform,
  PermissionsAndroid,
} from 'react-native';

import BleManager, {
  BleDisconnectPeripheralEvent,
  BleManagerDidUpdateValueForCharacteristicEvent,
  BleScanCallbackType,
  BleScanMatchMode,
  BleScanMode,
  Peripheral,
  PeripheralInfo
} from 'react-native-ble-manager';

const SECONDS_TO_SCAN_FOR = 10;
const SERVICE_UUIDS: string[] = ["6364e354-24f2-4048-881f-4943362d34d7"];
const ALLOW_DUPLICATES = true;

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

declare module 'react-native-ble-manager' {
  interface Peripheral {
    connected?: boolean;
    connecting?: boolean;
  }
}

export default function useBluetooth (){
  const [isScanning, setIsScanning] = useState(false);
  const [peripherals, setPeripherals] = useState(
    new Map<Peripheral['id'], Peripheral>(),
  );
  const [associatedPeripherals, setAssociatedPeripherals] = useState<Map<string, Peripheral> | null>(null);

  const startScan = () => {
    setPeripherals(new Map<Peripheral['id'], Peripheral>());
    if (!isScanning) {
      try {
        console.debug('[startScan] starting scan...');
        setIsScanning(true);
        BleManager.scan(SERVICE_UUIDS, SECONDS_TO_SCAN_FOR, ALLOW_DUPLICATES, {
          matchMode: BleScanMatchMode.Sticky,
          scanMode: BleScanMode.LowLatency,
          callbackType: BleScanCallbackType.AllMatches,
        })
          .then(() => {
            console.debug('[startScan] scan promise returned successfully.');
          })
          .catch((err: any) => {
            console.error('[startScan] ble scan returned in error', err);
          });
      } catch (error) {
        console.error('[startScan] ble scan error thrown', error);
      }
    }
  };

  const startCompanionScan = () => {
    setPeripherals(new Map<Peripheral['id'], Peripheral>());
    try {
      console.debug('[startCompanionScan] starting companion scan...');
      return BleManager.companionScan(SERVICE_UUIDS, { single: false })
        .then((peripheral: Peripheral|null) => {
          console.debug('[startCompanionScan] scan promise returned successfully.', peripheral);

          if(!peripheral)
            return;

          setPeripherals(map => {
            return new Map(map.set(peripheral.id, peripheral));
          });
        })
        .catch((err: any) => {
          console.debug('[startCompanionScan] ble scan cancel', err);
        });
    } catch (error) {
      console.error('[startCompanionScan] ble scan error thrown', error);
    }
  }

  const handleStopScan = () => {
    setIsScanning(false);
    console.debug('[handleStopScan] scan is stopped.');
  };

  const handleDisconnectedPeripheral = (
    event: BleDisconnectPeripheralEvent,
  ) => {
    console.debug(
      `[handleDisconnectedPeripheral][${event.peripheral}] disconnected.`,
    );
    setPeripherals(map => {
      let p = map.get(event.peripheral);
      if (p) {
        p.connected = false;
        return new Map(map.set(event.peripheral, p));
      }
      return map;
    });
  };

  const handleConnectPeripheral = (event: any) => {
    console.log(`[handleConnectPeripheral][${event.peripheral}] connected.`);
  };

  const handleUpdateValueForCharacteristic = (
    data: BleManagerDidUpdateValueForCharacteristicEvent,
  ) => {
    console.debug(
      `[handleUpdateValueForCharacteristic] received data from '${data.peripheral}' with characteristic='${data.characteristic}' and value='${data.value}'`,
    );
  };

  const handleDiscoverPeripheral = (peripheral: Peripheral) => {
    console.debug('[handleDiscoverPeripheral] new BLE peripheral=', peripheral);
    if (!peripheral.name) {
      peripheral.name = 'NO NAME';
    }
    setPeripherals(map => {
      return new Map(map.set(peripheral.id, peripheral));
    });
  };

  const disconnectPeripheral = async (peripheral: Peripheral) => {
    if (peripheral && peripheral.connected) {
      try {
        await BleManager.disconnect(peripheral.id);
      } catch (error) {
        console.error(
          `[togglePeripheralConnection][${peripheral.id}] error when trying to disconnect device.`,
          error,
        );
      }
    }
  };

  const getAssociatedPeripherals = async () => {
    try {
      const associatedPeripherals = await BleManager.getAssociatedPeripherals();
      console.debug(
        '[getAssociatedPeripherals] associatedPeripherals',
        associatedPeripherals,
      );

      if(associatedPeripherals.length == 0)
      {
        setAssociatedPeripherals(_ => new Map<string, Peripheral>());
        return;
      }

      for (let peripheral of associatedPeripherals) {
        setAssociatedPeripherals(map => {
          const retMap = map === null ? new Map<string, Peripheral>() : map;
          return new Map(retMap.set(peripheral.id, peripheral));
        });
      }
    } catch (error) {
      console.error(
        '[getAssociatedPeripherals] unable to retrieve associated peripherals.',
        error,
      );
    }
  }

  const connectPeripheral = async (peripheral: Peripheral) => {
    try {
      if (!peripheral)
        return;

      setPeripherals(map => {
        let p = map.get(peripheral.id);
        if (p) {
          p.connecting = true;
          return new Map(map.set(p.id, p));
        }
        return map;
      });

      await BleManager.connect(peripheral.id);
      console.debug(`[connectPeripheral][${peripheral.id}] connected.`);

      setPeripherals(map => {
        let p = map.get(peripheral.id);
        if (p) {
          p.connecting = false;
          p.connected = true;
          return new Map(map.set(p.id, p));
        }
        return map;
      });

      // before retrieving services, it is often a good idea to let bonding & connection finish properly
      await sleep(900);

      /* Test read current RSSI value, retrieve services first */
      const peripheralData = await BleManager.retrieveServices(peripheral.id);
      console.debug(
        `[connectPeripheral][${peripheral.id}] retrieved peripheral services`,
        peripheralData,
      );

      setPeripherals(map => {
        let p = map.get(peripheral.id);
        if (p) {
          return new Map(map.set(p.id, p));
        }
        return map;
      });
      
      const rssi = await BleManager.readRSSI(peripheral.id);
      console.debug(
        `[connectPeripheral][${peripheral.id}] retrieved current RSSI value: ${rssi}.`,
      );

      setPeripherals(map => {
        let p = map.get(peripheral.id);
        if (p) {
          p.rssi = rssi;
          return new Map(map.set(p.id, p));
        }
        return map;
      });
    } catch (error) {
      console.error(
        `[connectPeripheral][${peripheral.id}] connectPeripheral error`,
        error,
      );
    }
  };

  const retrieveConnected = async () => {
    try {
      const connectedPeripherals = await BleManager.getConnectedPeripherals();
      if (connectedPeripherals.length === 0) {
        console.warn('[retrieveConnected] No connected peripherals found.');
        return;
      }

      console.debug(
        '[retrieveConnected] connectedPeripherals',
        connectedPeripherals,
      );

      for (let peripheral of connectedPeripherals) {
        setPeripherals(map => {
          let p = map.get(peripheral.id);
          if (p) {
            p.connected = true;
            return new Map(map.set(p.id, p));
          }
          return map;
        });
      }
    } catch (error) {
      console.error(
        '[retrieveConnected] unable to retrieve connected peripherals.',
        error,
      );
    }
  }

  function sleep(ms: number) {
    return new Promise<void>(resolve => setTimeout(resolve, ms));
  }

  useEffect(() => {
    try {
      BleManager.start({ showAlert: false })
        .then(() => console.debug('BleManager started.'))
        .catch((error: any) =>
          console.error('BeManager could not be started.', error),
        );
    } catch (error) {
      console.error('unexpected error starting BleManager.', error);
      return;
    }

    const listeners = [
      bleManagerEmitter.addListener(
        'BleManagerDiscoverPeripheral',
        handleDiscoverPeripheral,
      ),
      bleManagerEmitter.addListener('BleManagerStopScan', handleStopScan),
      bleManagerEmitter.addListener(
        'BleManagerDisconnectPeripheral',
        handleDisconnectedPeripheral,
      ),
      bleManagerEmitter.addListener(
        'BleManagerDidUpdateValueForCharacteristic',
        handleUpdateValueForCharacteristic,
      ),
      bleManagerEmitter.addListener(
        'BleManagerConnectPeripheral',
        handleConnectPeripheral,
      ),
    ];

    handleAndroidPermissions();

    return () => {
      console.debug('[app] main component unmounting. Removing listeners...');
      for (const listener of listeners) {
        listener.remove();
      }
    };
  }, []);

  const handleAndroidPermissions = () => {
    if (Platform.OS === 'android' && Platform.Version >= 31) {
      PermissionsAndroid.requestMultiple([
        PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
        PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
      ]).then(result => {
        if (result) {
          console.debug(
            '[handleAndroidPermissions] User accepts runtime permissions android 12+',
          );
        } else {
          console.error(
            '[handleAndroidPermissions] User refuses runtime permissions android 12+',
          );
        }
      });
    } else if (Platform.OS === 'android' && Platform.Version >= 23) {
      PermissionsAndroid.check(
        PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
      ).then(checkResult => {
        if (checkResult) {
          console.debug(
            '[handleAndroidPermissions] runtime permission Android <12 already OK',
          );
        } else {
          PermissionsAndroid.request(
            PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
          ).then(requestResult => {
            if (requestResult) {
              console.debug(
                '[handleAndroidPermissions] User accepts runtime permission android <12',
              );
            } else {
              console.error(
                '[handleAndroidPermissions] User refuses runtime permission android <12',
              );
            }
          });
        }
      });
    }
  };

  return { isScanning, peripherals, associatedPeripherals, startScan, startCompanionScan, disconnectPeripheral, getAssociatedPeripherals, connectPeripheral, retrieveConnected };
}