import { Platform, NativeModules } from 'react-native';

import type { Spec } from './NativeJsiModule';

const LINKING_ERROR =
  `The package 'react-native-jsi-module' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

const isTurboModuleEnabled = true;

const JsiModuleModule = isTurboModuleEnabled
  ? require('./NativeJsiModule').default
  : NativeModules.JsiModule;

const JsiModule: Spec = JsiModuleModule
  ? JsiModuleModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

// @ts-expect-error
if (!global.__myModule__) {
  console.log('Installing JsiModule...');
  JsiModule.install();
  // @ts-expect-error
  console.log('JsiModule installed:', global.__myModule__);
}
// @ts-expect-error
const module = global.__myModule__;
console.log('module:', module);

export function reverseString(str: string): string {
  return module.reverseString(str);
}

export function getNumbers(): Array<number> {
  return module.getNumbers();
}

export function getObject(): Record<string, string> {
  return module.getObject();
}

export function callMeLater(
  successCB: () => void,
  failureCB: (error: Error) => void
) {
  module.callMeLater(successCB, failureCB);
}
export function promiseNumber(num: number) {
  return module.promiseNumber(num);
}
// Add the linearRegression function
export function linearRegression(
  features: number[],
  weights: number[]
): number {
  return module.linearRegression(features, weights);
}
