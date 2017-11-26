# Terra-Particle project

This is a Particle-based terrarium controller implemented on a RedBear Duo board.
More on Particle: https://github.com/spark

## Libraries used
https://github.com/vitiral/DHT

### Issues/things to fix

  State machine for LCD display
  (if scrolling through different sensors)

  every time the relays are toggled get next_toggle time for relays
  need to hold current state for relays. server could send next state
  for relays

  Get current time
  Serial.println(Time.format("%Y-%m-%d %H:%M:%S"));

  Need to add Daylight Savings Time (DST) support. Currently it is done manually

  ** I can also put leds on data lines to visualise activity.
  ** Must be sinked to better visualize data stream
  ** Add 3.3V support/selectors on sensor sockets.

  Particle.function turn device 1 (){
    if(!relay_state[0])
      turn on relay 1
    else
      turn off relay 1
  }

  When the MCU loads/resets it has to connect to the server and retrieve relay
  states for devices.

  get a 0x00 value for all devices. need an array to hold states for each of the
  devices?

  Let's start off simple - the server tells the MCU when to turn on and when
  to turn off. No need to download next_on/off or next_toggle times. When the
  time to turn off comes - the server turns something on ar off. Simple as
  that.

  When MCU loads it has to download current data of what devices have to be
  turned on. This is saved in the relay_states array. Additionally, MCU has
  to download the relay_next_on times and relay_next_off times, which are
  saved in the relay_next_on and relay_next_off arrays.

  every minute need to check if the time is equal to one of the times saved in
  relay_next_on and relay_next_off arrays. if yes, need to toggle the

  array to hold relay_states []
  array to hold relay_next_toggle times[]