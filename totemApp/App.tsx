import { NavigationContainer, ParamListBase } from "@react-navigation/native";
import { NativeStackScreenProps, createNativeStackNavigator } from "@react-navigation/native-stack";
import Home from "./src/screens/HomeScreen";
import { PeripheralInfo } from "react-native-ble-manager";

const Stack = createNativeStackNavigator<RootStackParamList>();

export type RootStackParamList = {
  Home: undefined,
  ScanDevices: undefined;
  PeripheralDetails: { peripheralData: PeripheralInfo };
};

export default function App() {
  return (
    <NavigationContainer>
        <Stack.Navigator>
            <Stack.Screen
                name="Home"
                component={Home}
            />
        </Stack.Navigator>
    </NavigationContainer>
  );
}
