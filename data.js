var mqtt = require('mqtt');
const fs = require('fs');
var obj = { 'username': 'user', 'password': 'password' }
var client = mqtt.connect('mqtt://test.mosquitto.org:1883', obj);

const Influx = require('influxdb-nodejs');
const clientInflux = new Influx('http://127.0.0.1:8086/aqDB');
const fieldSchema = {
aq: 'i',
ppm:'f'
};
const tagSchema = {
deviceId:'*'
};
clientInflux.schema('sendData', fieldSchema, tagSchema, {
// default is false
stripUnknown: true,
});

client.on('connect', () => {
console.log('Connected to server');
client.subscribe('sendData');
});
client.on('close', () => {
console.log('Disconnected from server');
});

client.on('message', (topic, message) => {
console.log("mqtt msg : " + message.toString());
data = JSON.parse(message)
if (data.deviceId && data.data) {
clientInflux.write('airData')
.tag({
deviceId: data.deviceId,
})
.field({
aq: data.data.rzero,
ppm:data.data.ppm
})
.then(() => {
console.log("Success")
})
.catch((err)=>{
console.log(err)
});
}
else
{
console.log("")
}
});
