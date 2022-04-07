#include "TemperatureHumidityValidatorRK.h"

#include <math.h>
#include <cfloat>

TemperatureHumidityValidator::TemperatureHumidityValidator() {

}

TemperatureHumidityValidator::~TemperatureHumidityValidator() {

}

void TemperatureHumidityValidator::addSample(float temperatureC, float humidity) {
    if (isnan(temperatureC) || isnan(humidity) ||
        humidity < 0 || humidity > 100.0) {
        // temperatureC being nan and humidity are common error conditions so
        // throw out the sample
        // Log.info("discarding temperatureC=%f humidity=%f", temperatureC, humidity);
        return;
    }

    // Log.info("adding sample temperatureC=%f humidity=%f", temperatureC, humidity);

    tempSamples[numSamples % NUM_SAMPLES] = temperatureC;
    humSamples[numSamples % NUM_SAMPLES] = humidity;
    numSamples++;
}


float TemperatureHumidityValidator::getTemperatureC() const {
    if (numSamples < NUM_SAMPLES) {
        // Don't have enough samples yet
        return nanf("");
    }

    return getValue(tempSamples);
}

float TemperatureHumidityValidator::getTemperatureF() const {
    float tempC = getTemperatureC();
    if (!isnan(tempC)) {
        return (tempC * 9.0) / 5.0 + 32.0;
    }
    else {
        return nanf("");
    }
}

float TemperatureHumidityValidator::getHumidity() const {
    if (numSamples < NUM_SAMPLES) {
        // Don't have enough samples yet
        return nanf("");
    }
    return getValue(humSamples);
}

// [static]
float TemperatureHumidityValidator::getValue(const float *samples) {
    // The way the bit shift happens is that some values tend to be basically 2x the
    // expected value. 

    // Calculate mean
    float sum = 0.0;

    for(size_t ii = 0; ii < NUM_SAMPLES; ii++) {
        sum += samples[ii];
    }

    float mean = sum / NUM_SAMPLES;

    // Calculate the standard deviation
    float sumDeviationSquared = 0.0;

    for(size_t ii = 0; ii < NUM_SAMPLES; ii++) {
        float deviation = samples[ii] - mean;
        sumDeviationSquared += (deviation * deviation);
    }

    float sd = sqrt(sumDeviationSquared / (NUM_SAMPLES - 1));

    // Now accumulate values within 1 standard deviation

    float sumTempSamples = 0.0;
    size_t numTempSamples = 0;

    for(size_t ii = 0; ii < NUM_SAMPLES; ii++) {
        if ((mean - sd) <= samples[ii] && samples[ii] <= (mean + sd)) {
            sumTempSamples += samples[ii];
            numTempSamples++;
        }
    }

    // Returned value is the mean of the middle quartile samples
    float result = sumTempSamples / numTempSamples;

    // Log.info("result=%f mean=%f sd=%f", result, mean, sd);

    return result;
}



/*

0000002040 [app] INFO: loading config location: {"version":1,"hash":"5835F2693A2EE523DC71F1871F1E2465","location":{"radius":0,"interval_min":30,"interval_max":60,"min_publish":false~
0000002049 [app] INFO: loading config imu_trig: {"version":1,"hash":"5E5488D40858A17D5C214BA3319647FD","imu_trig":{"motion":"disable","high_g":"disable"}}
0000002066 [app] INFO: loading config rgb: {"version":1,"hash":"2B1CD6509BBD4801D3AF1DE32E1A9D23","rgb":{"type":"particle","direct":{"brightness":255,"red":0,"green":0,"blue":255}}}
0000002072 [ncp.client] TRACE: Modem already on
0000002073 [gsm0710muxer] INFO: Starting GSM07.10 muxer
0000002074 [gsm0710muxer] INFO: Openning mux channel 0
0000002074 [gsm0710muxer] INFO: GSM07.10 muxer thread started
0000002077 [gsm0710muxer] INFO: Openning mux channel 1
0000002129 [ncp.client] TRACE: NCP ready to accept AT commands
0000002130 [gsm0710muxer] INFO: Mux channel 1 already opened
0000002133 [ncp.client] TRACE: NCP state changed: 1
0000002134 [net.pppncp] TRACE: NCP event 1
0000002168 [ncp.client] TRACE: NCP connection state changed: 1
0000002169 [net.pppncp] TRACE: NCP event 2
0000002170 [net.pppncp] TRACE: State changed event: 1
0000002345 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000002347 [app] INFO: tempC=nan tempF=nan humidity=nan
0000002486 [ncp.client] TRACE: NCP connection state changed: 2
0000002487 [gsm0710muxer] INFO: Openning mux channel 2
0000002535 [net.pppncp] TRACE: NCP event 100
0000002536 [net.pppncp] TRACE: New auth info
0000002537 [net.pppncp] TRACE: NCP event 2
0000002538 [net.pppncp] TRACE: State changed event: 2
0000003070 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000003071 [app] INFO: tempC=nan tempF=nan humidity=nan
0000003557 [net.pppncp] TRACE: Negotiated MTU: 1500
0000003559 [system] INFO: Cloud: connecting
0000003564 [system] INFO: Cloud socket connected
0000003566 [comm.protocol.handshake] INFO: Establish secure connection
0000004014 [comm.protocol.handshake] INFO: Sending HELLO message
0000004347 [app] INFO: discarding temperatureC=nan humidity=41.000000
0000004349 [app] INFO: tempC=nan tempF=nan humidity=nan
0000005071 [app] INFO: adding sample temperatureC=0.000000 humidity=41.000000
0000005073 [app] INFO: tempC=nan tempF=nan humidity=nan
0000005171 [comm.protocol.handshake] INFO: Handshake completed
0000005173 [comm.protocol.handshake] TRACE: Updating protocol flags
0000005597 [comm.protocol] INFO: Posting 'S' describe message
0000005616 [comm.protocol] INFO: Posting 'A' describe message
0000005620 [comm.protocol] INFO: Sending subscriptions
0000005993 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000005995 [comm.protocol] TRACE: message id 340 complete with code 0.00
0000005997 [comm.protocol] TRACE: Updating system DESCRIBE checksum
0000006079 [app] INFO: {"cmd":"loc","time":1595863707,"loc":{"lck":1,"time":1595863708,"lat":42.33393860,"lon":-75.31638336,"alt":358.762,"hd":232.28,"h_acc":31.000,"v_acc":58.000,"~
0000006088 [app] INFO: cloud sent: {"cmd":"loc","time":1595863707,"loc":{"lck":1,"time":1595863708,"lat":42.33393860,"lon":-75.31638336,"alt":358.762,"hd":232.28,"h_acc":31.000,"v_a~
0000006272 [comm.protocol] TRACE: rcv'd message type=13
0000006274 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006275 [comm.protocol] TRACE: message id 341 complete with code 0.00
0000006276 [comm.protocol] TRACE: rcv'd message type=13
0000006292 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006293 [comm.protocol] TRACE: message id 342 complete with code 0.00
0000006295 [comm.protocol] TRACE: rcv'd message type=13
0000006364 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006366 [comm.protocol] TRACE: message id 343 complete with code 0.00
0000006368 [comm.protocol] TRACE: rcv'd message type=13
0000006370 [app] INFO: discarding temperatureC=nan humidity=45.000000
0000006371 [app] INFO: tempC=nan tempF=nan humidity=nan
0000006372 [app] INFO: location cb publish 6 failure
0000006395 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006397 [comm.protocol] TRACE: message id 344 complete with code 0.00
0000006399 [comm.protocol] TRACE: rcv'd message type=13
0000006471 [comm.protocol] TRACE: Reply recieved: type=2, code=69
0000006473 [comm.protocol] TRACE: message id 345 complete with code 2.05
0000006475 [comm.protocol] INFO: Received TIME response: 1595863708
0000006479 [comm.protocol] TRACE: rcv'd message type=12
0000006519 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006521 [comm.protocol] TRACE: message id 346 complete with code 0.00
0000006523 [comm.protocol] TRACE: Updating application DESCRIBE checksum
0000006526 [comm.protocol] TRACE: rcv'd message type=13
0000006629 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006631 [comm.protocol] TRACE: message id 347 complete with code 0.00
0000006633 [comm.protocol] TRACE: rcv'd message type=13
0000006735 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000006737 [comm.protocol] TRACE: message id 348 complete with code 0.00
0000006739 [comm.protocol] TRACE: Updating subscriptions checksum
0000006742 [comm.protocol] TRACE: rcv'd message type=13
0000006844 [system] INFO: Cloud connected
0000007000 [app] INFO: cloud sent: {"cmd":"sync","time":1595863708,"hash":"921DF9A1DE402C9ABE301B776EC6E2D5"}
0000007094 [app] INFO: adding sample temperatureC=0.000000 humidity=45.000000
0000007097 [app] INFO: tempC=nan tempF=nan humidity=nan
0000007395 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000007397 [comm.protocol] TRACE: message id 349 complete with code 0.00
0000007400 [comm.protocol] TRACE: rcv'd message type=13
0000008070 [app] INFO: cloud sent: {"cmd":"loc","time":1595863707,"loc":{"lck":1,"time":1595863708,"lat":42.33393860,"lon":-75.31638336,"alt":358.762,"hd":232.28,"h_acc":31.000,"v_a~
0000008369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000008370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000008749 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000008751 [comm.protocol] TRACE: message id 350 complete with code 0.00
0000008755 [comm.protocol] TRACE: rcv'd message type=13
0000009094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000009095 [app] INFO: tempC=nan tempF=nan humidity=nan
0000009248 [comm.protocol] TRACE: rcv'd message type=2
0000009248 [app] INFO: cloud received: {"cmd":"ack","req_id":1,"src_cmd":"loc","status":0}
0000009250 [app] INFO: location cb publish 6 success!
0000009570 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000009572 [comm.protocol] TRACE: message id 351 complete with code 0.00
0000009575 [comm.protocol] TRACE: rcv'd message type=13
0000010369 [app] INFO: discarding temperatureC=nan humidity=45.000000
0000010370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000011094 [app] INFO: adding sample temperatureC=30.000000 humidity=45.000000
0000011095 [app] INFO: tempC=nan tempF=nan humidity=nan
0000012119 [comm.protocol] INFO: Received DESCRIBE request; flags: 0x04
0000012145 [comm.protocol] INFO: Posting 'M' describe message
0000012154 [comm.protocol] TRACE: rcv'd message type=1
0000012369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000012370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000012551 [comm.protocol] TRACE: Reply recieved: type=2, code=0
0000012553 [comm.protocol] TRACE: message id 352 complete with code 0.00
0000012555 [comm.protocol] TRACE: rcv'd message type=13
0000013094 [app] INFO: adding sample temperatureC=127.000000 humidity=44.000000
0000013095 [app] INFO: tempC=nan tempF=nan humidity=nan
0000014369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000014370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000015094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000015095 [app] INFO: tempC=nan tempF=nan humidity=nan
0000016369 [app] INFO: discarding temperatureC=nan humidity=40.000000
0000016370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000017094 [app] INFO: adding sample temperatureC=30.000000 humidity=40.000000
0000017095 [app] INFO: tempC=nan tempF=nan humidity=nan
0000018369 [app] INFO: discarding temperatureC=nan humidity=40.000000
0000018370 [app] INFO: tempC=nan tempF=nan humidity=nan
0000019094 [app] INFO: adding sample temperatureC=30.000000 humidity=40.000000
0000019095 [app] INFO: tempC=23.333334 tempF=74.000000 humidity=44.285713
0000020369 [app] INFO: discarding temperatureC=nan humidity=40.000000
0000020370 [app] INFO: tempC=23.333334 tempF=74.000000 humidity=44.285713
0000021094 [app] INFO: adding sample temperatureC=30.000000 humidity=40.000000
0000021095 [app] INFO: tempC=23.333334 tempF=74.000000 humidity=43.400002
0000022369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000022370 [app] INFO: tempC=23.333334 tempF=74.000000 humidity=43.400002
0000023094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000023095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.285713
0000024369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000024370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000025094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000025095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000026369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000026370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000027094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000027095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000028369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000028370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000029094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000029095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000030369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000030370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000031094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000031095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000032369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000032370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000033094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000033095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000034369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000034370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000035094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000035095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000036372 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000036373 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000037094 [app] INFO: adding sample temperatureC=60.000000 humidity=44.000000
0000037095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000038370 [app] INFO: discarding temperatureC=nan humidity=192.000000
0000038371 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000039094 [app] INFO: discarding temperatureC=1.000000 humidity=192.000000
0000039095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000040369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000040370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000041094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000041095 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000042369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000042370 [app] INFO: tempC=30.000000 tempF=86.000000 humidity=44.000000
0000043094 [app] INFO: adding sample temperatureC=127.000000 humidity=44.000000
0000043095 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000044369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000044370 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000045094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000045095 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000046369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000046370 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000047094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000047095 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000048369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000048370 [app] INFO: tempC=33.333332 tempF=92.000000 humidity=44.000000
0000049094 [app] INFO: adding sample temperatureC=60.000000 humidity=44.000000
0000049095 [app] INFO: tempC=36.666668 tempF=98.000000 humidity=44.000000
0000050369 [app] INFO: discarding temperatureC=nan humidity=44.000000
0000050370 [app] INFO: tempC=36.666668 tempF=98.000000 humidity=44.000000
0000051094 [app] INFO: adding sample temperatureC=127.000000 humidity=44.000000
0000051095 [app] INFO: tempC=37.500000 tempF=99.500000 humidity=44.000000
0000052369 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000052370 [app] INFO: tempC=37.500000 tempF=99.500000 humidity=44.000000
0000053094 [app] INFO: adding sample temperatureC=30.000000 humidity=44.000000
0000053095 [app] INFO: tempC=33.750000 tempF=92.750000 humidity=44.000000

*/