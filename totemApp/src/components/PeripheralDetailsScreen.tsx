import { NativeStackScreenProps } from '@react-navigation/native-stack';
import React, { useState } from 'react';
import { View, TextInput, StyleSheet, Button } from 'react-native';
import BleManager from 'react-native-ble-manager';
import { RootStackParamList } from '../../App';
import { Buffer } from "buffer";

type Props = NativeStackScreenProps<RootStackParamList, 'PeripheralDetails'>;

const PeripheralDetailsScreen = ({navigation, route}: Props) => {
  const [totemText, setTotemText] = useState("");
  const peripheralData = route.params.peripheralData;

  const writeData = (text : string): void => {
    const buffer = Buffer.from(text);
    BleManager.write(peripheralData.id, "6364e354-24f2-4048-881f-4943362d34d7", "7504932b-317e-4bb8-9998-5d2f5f3d18b3", buffer.toJSON().data);
  };
  
  return (
    <View>
      <TextInput placeholder='Say it wit yo chest' style={{color:"black"}} placeholderTextColor="black" value={totemText} onChangeText={text => setTotemText(text)}/>
      <Button title="Let the people know" onPress={() => writeData(totemText)}/>
    </View>
  );
};

// Add some basic styling
const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 20,
  },
  title: {
    fontSize: 20,
    fontWeight: 'bold',
  },
  titleWithMargin: {
    marginTop: 20, // Adjust this value as needed
  },
  detail: {
    marginTop: 5,
    fontSize: 16,
  },
  serviceContainer: {
    marginTop: 15,
  },
  serviceTitle: {
    fontSize: 18,
    fontWeight: 'bold',
  },
  characteristic: {
    fontSize: 16,
  },
  scrollViewStyle: {
    flex: 1,
    backgroundColor: '#069400'
  },
  contentContainer: {
    padding: 20,
  },
  characteristicContainer: {
    marginTop: 10,
  },
  characteristicTitle: {
    fontSize: 16,
    fontWeight: '500',
  },
  propertyText: {
    fontSize: 14,
    marginLeft: 10,
  },
});

export default PeripheralDetailsScreen;