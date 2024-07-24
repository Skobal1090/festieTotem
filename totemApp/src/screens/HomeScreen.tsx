import { useEffect } from "react";
import { View, Pressable, Text, StyleSheet, FlatList } from 'react-native';
import PeripheralComponent from "../components/PeripheralComponent";
import useBluetooth from "../hooks/useBluetooth";
import { Peripheral } from "react-native-ble-manager";
import { RootStackParamList } from "../../App";
import { NativeStackScreenProps } from '@react-navigation/native-stack';

type Props = NativeStackScreenProps<RootStackParamList, 'Home'>;

export default function Home({ navigation } : Props){
    const btManager = useBluetooth();

    useEffect(() => {
        const doSearch = async () => {
            if(btManager.associatedPeripherals === null){
                console.log("associated peripherals not retrieved yet, fetching...");
                await btManager.getAssociatedPeripherals();
                console.log("associated peripherals retrieved!");
                return;
            }

            if(btManager.associatedPeripherals.size > 0){
                console.log("associated peripherals is nonempty, starting scan...");
                await btManager.startScan();
                console.log("finished scan!");
                return;
            }
        };

        doSearch();
    }, [btManager.associatedPeripherals]);

    const connectDevice = async (periph: Peripheral) => {
        var info = await btManager.connectPeripheral(periph);

        if(info == null)
            return;
        
        console.log("navigating to details");
        navigation.navigate('PeripheralDetails', {
          peripheralData: info,
        });
    }

    const clearConnections = async () => {
        var devices = await btManager.retrieveConnected();

        let data : Peripheral;

        data = devices[0];

        for(let i = 0; i < devices.length; i++){
            await btManager.disconnectPeripheral(devices[i]);
        }
    };

    const renderPeripheral = ({ item } : { item : [string, Peripheral]}) => <PeripheralComponent data={item[1]} onPress={() => connectDevice(item[1])} />;

    return (
    <View style={styles.container}>
        <FlatList data={[...btManager.peripherals]} renderItem={renderPeripheral} />
        <Pressable style={styles.button} onPress={() => btManager.startCompanionScan()}>
            <Text style={styles.buttonLabel}>Initial Scan</Text>
        </Pressable>
        <Pressable style={styles.button} onPress={() => btManager.startScan()}>
            <Text style={styles.buttonLabel}>Scan for totem</Text>
        </Pressable>
        <Pressable style={styles.button} onPress={() => btManager.getAssociatedPeripherals()}>
            <Text style={styles.buttonLabel}>List previously discovered</Text>
        </Pressable>
        <Pressable style={styles.button} onPress={() => clearConnections()}>
            <Text style={styles.buttonLabel}>Cleanup Connections</Text>
        </Pressable>
    </View>
    );
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: "#fff",
        alignItems: "center",
        justifyContent: "center"
    },
    button: {
        backgroundColor: "#003a9f",
        padding: 10,
        borderRadius: 5
    },
    buttonLabel: {
        color: "white"
    }
});