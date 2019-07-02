<template>

    <v-container grid-list-md text-xs-center>

        <v-layout row wrap>

            <v-flex xs6 v-for="sensor in sensors" :key="sensor.sensor">

                <v-card class="mx-auto"
                        color="grey lighten-4">

                    <v-card-title primary-title class="px-0">


<!--
                        <ThermometerIcon class="icon" width="64px" height="64px" style="fill: #e57373"/>
                        -->
                        <v-icon
                                color="red lighten-2"
                                class="mr-5"
                                size="64"
                        >{{ sensor.icon }}
                        </v-icon>

                        <div>
                            <h3 class="headline mb-0">{{ sensor.sensor }}&nbsp;</h3>
                        </div>

                        <v-layout column align-start>
                            <div class="caption grey--text text-uppercase">
                                {{ sensor.sensortype }}
                            </div>
                            <div>
                                <span class="display-2 font-weight-black"
                                      v-text="sensor.current_value || '—'">
                                </span>
                                <strong>{{ sensor.unit }}</strong>
                            </div>
                        </v-layout>

                        <v-spacer></v-spacer>

                        <v-btn icon class="align-self-start" size="28">
                            <v-icon>info</v-icon>
                        </v-btn>

                    </v-card-title>

                    <!-- use span to work around the label-size bug with the trend v-sparkline -->
                    <span style="font-size: 11px">
                        <v-sparkline
                                :value="sensor.data"
                                :labels="labels"
                                :label-size="labelSize"
                                :gradient="gradient"
                                :gradient-direction="gradientDirection"
                                :smooth="radius || false"
                                :line-width="width"
                                :stroke-linecap="lineCap"
                                padding="8">

                        </v-sparkline>
                    </span>

                    <v-card-text class="pt-0">
                        <v-divider class="my-2"></v-divider>
                        <v-icon class="mr-2"
                                small>
                            access_time
                        </v-icon>
                        <span class="caption grey--text font-weight-light">
                            last update 26 minutes ago
                        </span>
                    </v-card-text>

                </v-card>

            </v-flex>

        </v-layout>

    </v-container>
</template>

<script>
    import axios from 'axios';
    // import ThermometerIcon from '../assets/thermometer-lines.vue';

    export default {
        data: () => ({
            gradient: ['red', 'yellow', 'blue'],
            gradientDirection: 'top',
            radius: 10,
            width: 1,
            lineCap: 'round',
            labelSize: 0.01,
            labels: ['4pm', '8pm', '12am', '4am', '8am', '12pm'],
            sensors: []
        }),
          mounted () {
            axios
              .get('./api/greenhouse/sensors')
              .then(response => (this.sensors = response.data))
              .catch(error => console.log(error))
          }

    }
</script>

<style>

</style>
