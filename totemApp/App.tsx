import { NavigationContainer, ParamListBase } from "@react-navigation/native";
import { NativeStackScreenProps, createNativeStackNavigator } from "@react-navigation/native-stack";
import ScanDevicesScreen from "./components/ScanDevicesScreen";
import PeripheralDetailsScreen from "./components/PeripheralDetailsScreen";
import { PeripheralInfo } from "react-native-ble-manager";

const Stack = createNativeStackNavigator<RootStackParamList>();

export type RootStackParamList = {
  ScanDevices: undefined;
  PeripheralDetails: { peripheralData: PeripheralInfo };
};

export default function App() {
  return (
    <NavigationContainer>
        <Stack.Navigator>
            <Stack.Screen
                name="ScanDevices"
                component={ScanDevicesScreen}
            />
            <Stack.Screen
                name="PeripheralDetails"
                component={PeripheralDetailsScreen}
            />
        </Stack.Navigator>
    </NavigationContainer>
  );
}
