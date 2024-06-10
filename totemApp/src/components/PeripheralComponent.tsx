import { Pressable, View, Text, StyleSheet } from "react-native";
import { Peripheral } from "react-native-ble-manager";

type PeripheralProps = {
    data: Peripheral,
    onPress: () => void
}

export default function PeripheralComponent (props : PeripheralProps){
    return (
        <Pressable onPress={props.onPress}>
            <View style={styles.card}>
                <Text style={styles.header}>{props.data.name}</Text>
                <Text style={styles.detail}>Id: {props.data.id}</Text>
                <Text style={styles.detail}>Rssi: {props.data.rssi}</Text>
            </View>
        </Pressable>
    );
}

const styles = StyleSheet.create({
    header: {
        color: "black",
        fontSize: 16,
        fontWeight: "bold"
    },
    detail: {
        color: "black",
        fontSize: 12
    },
    card: {
        backgroundColor: "#b2b2b2",
        borderRadius: 15,
        padding: 10
    }
});