import { View, StyleSheet, Button } from 'react-native';
import {
  callMeLater,
  getNumbers,
  getObject,
  promiseNumber,
  reverseString,
  linearRegression,
} from 'react-native-jsi-module';

export default function App() {
  const features = [2.0, 4.0, 6.0];
  const weights = [0.5, 1.0, 1.5, 9.0]; // Including the intercept

  return (
    <View style={styles.container}>
      <Button
        title="reverse string"
        onPress={() => {
          console.log(reverseString('reverse string'));
        }}
      />
      <Button
        title="get number"
        onPress={() => {
          console.log(getNumbers());
        }}
      />
      <Button
        title="get object"
        onPress={() => {
          console.log(getObject());
        }}
      />
      <Button
        title="promise"
        onPress={async () => {
          const value = await promiseNumber(5);

          console.log('promised value is: ', value);
        }}
      />
      <Button
        title="callbacks"
        onPress={() => {
          callMeLater(
            () => console.log('success'),
            () => console.log('failure')
          );
        }}
      />
      <Button
        title="linear regression"
        onPress={() => {
          console.log(linearRegression(features, weights));
        }}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: 'white',
  },
});
