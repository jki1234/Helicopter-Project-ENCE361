ENCE361: Helicopter Rig Control Project: Milestone 1
Milestone 1 involves generating a TIVA program to respond reliably and in real-time to button
pushes and a single analogue input. The analogue input is from the helicopter altitude sensor,
which always outputs in the approximate range of 1 V to 2 V. The voltage reduces by 1.0 V as
the helicopter rises from landed (0 %) to fully up (100 %). You should assume signal frequency
components above 4 Hz are noise.
In weeks 4 and 5 you will have access in the Electronics lab (during lab sessions) or in the
Embedded Systems Lab (at all times) to helicopter test mounts, which each incorporate the
same altitude sensor as the remote lab rigs. However, you should design your program so that
you can first test it using the potentiometer on the Orbit board, which is connected to ‘AIN0‘
and is used in the Week 4 lab. Milestone 1 will be assessed using one of the helicopter test
mounts. The specification for Milestone 1 is as follows:

The input on ‘AIN9‘, ‘PE4‘ (J1-05), namely the helicopter altitude sensor signal, should
be sampled at regular intervals.
The samples (i.e. ADC outputs) should be stored continually in a circular buffer. At
regular intervals the mean value of the samples in the buffer should be computed.
At the initiation of the program the mean sample value should be recorded as the number
corresponding to the “helicopter landed” altitude (0 %). This is required for the calculation
of the helicopter altitude.
From the time of initiation onwards (cf. M1.3) the helicopter altitude should be displayed
on the Orbit OLED display as a percentage, except as indicated in M1.6. 100 % should
be exactly 1.0 V below the recorded ”helicopter landed” altitude. Do not calibrate your
program to a specific test rig.
From the time of initiation onwards, operating the “LEFT” button, SW1 on the TIVA
board, should repeat the action detailed in M1.3.
A single push of the ”UP” button, BTN2 on the Orbit board, changes the display of
altitude from percentage altitude to the mean ADC value computed as in M1.2; a second
push turns the display blank; thereafter subsequent pushes make the display cycle through
the same three states: percentage altitude → mean ADC value → off.
The source code used to achieve Milestone 1 should be pushed to the group’s repository
on the same day that Milestone 1 is assessed.
If you want to progress towards Milestone 2, it is recommended that you branch your git
repository to ensure that your Milestone 1 code is maintained. Be sure to record information
about your design and how you have tried to meet the specification, as you will need this
information for the report due at the end of the project. Examples of the information you
should record include how you decided what sampling rate to use and the size of the buffer
to use. Groups will receive feedback on the quality of the Milestone 1 source code they have
pushed to their repository.
Note also that the helicopter test mounts should not be removed from their locations in the
Electronics Lab and Embedded Systems Lab.
NB: Your group’s program will only be tested on the specification above. Adding more
features or exceeding the specification is risky because it introduces greater potential for error.
