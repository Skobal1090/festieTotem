import { useEffect } from "react";
import { View, StyleSheet, FlatList } from 'react-native';
import PeripheralComponent from "../components/PeripheralComponent";
import useBluetooth from "../hooks/useBluetooth";
import { Peripheral } from "react-native-ble-manager";

export default function Home(){
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

    const renderPeripheral = ({ item } : { item : [string, Peripheral]}) => <PeripheralComponent data={item[1]} onPress={() => btManager.connectPeripheral(item[1])} />;

    return (
    <View style={styles.container}>
        <FlatList data={[...btManager.peripherals]} renderItem={renderPeripheral} />
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