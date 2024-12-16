#include "react-native-jsi-module.h"
#include <jsi/jsi.h>
#include <vector>
using namespace facebook;
namespace jsimodule
{
    // Simple linear regression function
    double linearRegressionPredict(const std::vector<double> &features, const std::vector<double> &weights)
    {
        double prediction = weights[0]; // Intercept
        for (size_t i = 0; i < features.size(); ++i)
        {
            prediction += features[i] * weights[i + 1];
        }
        return prediction;
    }
    void install(jsi::Runtime &jsiRuntime)
    {
        jsi::Object myModule = jsi::Object(jsiRuntime); // const obj ={}

        jsi::Function reverseString = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "reverseString"),
            1, //  number of arguements for function
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count // number of arguement passed
                ) -> jsi::Value
            {
                if (count != 1 || !arguments[0].isString())
                {
                    throw jsi::JSError(runtime, "Arguement has to be a string");
                }
                std::string str = arguments[0].asString(runtime).utf8(runtime);
            std:
                reverse(str.begin(), str.end());
                return jsi::String::createFromUtf8(runtime, str);
            });
        myModule.setProperty(jsiRuntime, "reverseString", reverseString);

        jsi::Function getNumbers = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "getNumbers"),
            0, //  number of arguements for function
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count // number of arguement passed
                ) -> jsi::Value
            {
                jsi::Array array = jsi::Array(runtime, 10);
                for (std::size_t i = 0; i < 10; i++)
                {
                    array.setValueAtIndex(runtime, i, jsi::Value(static_cast<int>(i)));
                }

                return array;
            });
        myModule.setProperty(jsiRuntime, "getNumbers", getNumbers);

        jsi::Function getObject = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "getObject"),
            0,
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count) -> jsi::Value
            {
                jsi::Object obj = jsi::Object(runtime);
                obj.setProperty(runtime, "result", jsi::String::createFromUtf8(runtime, "success"));
                return obj;
            });
        myModule.setProperty(jsiRuntime, "getObject", getObject);

        jsi::Function callMeLater = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "callMeLater"),
            2,
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count) -> jsi::Value
            {
                if (count != 2 || !arguments[0].isObject() || !arguments[0].asObject(runtime).isFunction(runtime) || !arguments[1].isObject() || !arguments[0].asObject(runtime).isFunction(runtime))
                {
                    throw jsi::JSError(runtime, "Expected two callback functions as arguments");
                };
                jsi::Function successCB = arguments[0].asObject(runtime).asFunction(runtime);
                jsi::Function failureCB = arguments[0].asObject(runtime).asFunction(runtime);

                bool callSuccess = std::rand() % 2;
                if (callSuccess)
                {
                    successCB.call(runtime);
                }
                else
                {
                    failureCB.call(runtime);
                }

                return jsi::Value::undefined();
            });
        myModule.setProperty(jsiRuntime, "callMeLater", callMeLater);

        jsi::Function promiseNumber = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "promiseNumber"),
            1, //  number of arguements for function
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count // number of arguement passed
                ) -> jsi::Value
            {
                if (count != 1 || !arguments[0].isNumber())
                {
                    throw jsi::JSError(runtime, "Expected one numeric argument");
                }

                double number = arguments[0].asNumber();

                jsi::Function promiseConstructor = runtime.global().getPropertyAsFunction(runtime, "Promise");

                return promiseConstructor.callAsConstructor(runtime,
                                                            jsi::Function::createFromHostFunction(
                                                                runtime,
                                                                jsi::PropNameID::forAscii(runtime, "promiseArg"),
                                                                2,
                                                                [number](
                                                                    jsi::Runtime &runtime,
                                                                    const jsi::Value &thisValue,
                                                                    const jsi::Value *arguments,
                                                                    std::size_t count) -> jsi::Value
                                                                {
                                                                    jsi::Function resolve = arguments[0].asObject(runtime).asFunction(runtime);
                                                                    resolve.call(runtime, number);
                                                                    return jsi::Value::undefined();
                                                                })

                );
            });
        myModule.setProperty(jsiRuntime, "promiseNumber", promiseNumber);

        // Expose linear regression prediction function
        jsi::Function linearRegression = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "linearRegression"),
            2, // number of arguments for function
            [](

                jsi::Runtime &runtime,
                const jsi::Value &thisValue,
                const jsi::Value *arguments,
                std::size_t count // number of arguments passed
                ) -> jsi::Value
            {
                if (count != 2 || !arguments[0].isObject() || !arguments[1].isObject())
                {
                    throw jsi::JSError(runtime, "Arguments must be two arrays");
                }

                jsi::Array featuresArray = arguments[0].asObject(runtime).asArray(runtime);
                jsi::Array weightsArray = arguments[1].asObject(runtime).asArray(runtime);

                std::vector<double> features;
                std::vector<double> weights;

                for (size_t i = 0; i < featuresArray.size(runtime); ++i)
                {
                    features.push_back(featuresArray.getValueAtIndex(runtime, i).asNumber());
                }

                for (size_t i = 0; i < weightsArray.size(runtime); ++i)
                {
                    weights.push_back(weightsArray.getValueAtIndex(runtime, i).asNumber());
                }

                double prediction = linearRegressionPredict(features, weights);
                return jsi::Value(prediction);
            });
        myModule.setProperty(jsiRuntime, "linearRegression", linearRegression);

        jsiRuntime.global().setProperty(jsiRuntime, "__myModule__", std::move(myModule)); // global.__myModule =obj
    }
}
