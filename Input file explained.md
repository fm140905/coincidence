<!--
 * @Description: Input options
 * @Author: Ming Fang
 * @Date: 2021-04-06 15:40:21
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-01-13 16:07:27
-->

## Structure
The input file is in relaxed json format. Comments can be added using `// comment` or `/*comment*/`. 

The json file consits of two parts: 
- `ChannelSettings`: for processing binary file of each channel. It consists of two sections:
    - `Defaults`: settings common for all channels, such as event structure, digitizer specs, etc.
    - `ChannelSpecific`: a list of channel-specific options, such as channel number, file path, etc. Note that options appearing here will overwite the same options in `Defaults`, making it convenient to adjust settings of a specific channel.
- `CoincidenceSettings`: for finding coincidences and ToF plots.

## Explanation of parameters
### Parsing
- `MaxNumPulses`: max number of pulses to process.
- `Polarity`: PMT polarity, either `positive` or `negative`.

- `Timestep`: digitizer time resolution in ns.
- `Resolution`: digitizer ADC resolution in bits.

- `Headers`: size of each header in bytes.
- `SampleSize`: size of each waveform sample in bytes.

- `Dynamicrange`: output dynamic range in volts.

- `BaselineSamples`: number of samples used to calculate the baseline

- `PreTrigger`: same as CoMPASS pretrigger, in ns
- `PreGate`: same as CoMPASS pre-gate, in ns
- `ShortGate`: same as CoMPASS short date, in ns
- `LongGate`: same as CoMPASS long gate, in ns

- `SavePulses`: number of pulses to save in a txt file.

### Pulse processing
- `Calibration`: energy calibration
    - `Integral`: calibrated based on pulse total integral.
    - `ON`: calibration based on integral if true
    - `Coefficient`: integral-to-energy linear calibration coefficient.
    - `Height`: calibrated based on pulse height.
    - `ON`: calibration based on height if true.
    - `Coefficient`: height-to-energy linear calibration coefficient.

- `Rejection`: pulse rejection.
    - `ON`: enable pulse rejection if true.
    - `ZeroSupression`: enable zero supression if true.
    - `LowVoltageThreshold`: minimum pulse height for zero supression.
    - `ClipRejection`: reject clipped pulse if true.
    - `PileupRejection`: piled-up pulse rejection
        - `ON`: enable piledup rejection if true
        - `RiseTime`: length of pulse rising edge, in ns.
        - `MinimumPeakRatio` and `MinimumPeakHeight`: pulse classified as piledup if ratio of two peaks > `MinimumPeakRatio` **AND** both peak heights > `MinimumPeakHeight`

- `EnergyCut`: energy cut. Calibration needs to be enabled. 
    - `ON`: enable energy cut if true.
    - `LowerThreshold`: lower energy threshold for energy cut.
    - `UpperThreshold`: upper energy threshold for energy cut.

- `PSDCut`: PSD cut. Calibration needs to be enabled. 
    - `ON`: enable PSD cut if true.
    - `LazyCut`: Perform straight line PSd cut if true. Similar to energy cut. Reject pulses above the upper threshold or below the lower threshold.
    - `LowerThreshold`: lower tail-total ratio threshold for lazy PSD cut.
    - `UpperThreshold`: upper tail-total ratio threshold for lazy PSD cut.
    - `QuadraticCut`: perform quadratic PSD cut if true. Reject pulses below the quadratic PSD line.
    - `QuadracticCoefficients`:  quadratic PSD line coefficients in order of quadratic coefficient, the linear coefficient and the constant or free term. Tail = [0] * Total^2 + [1] * Total + [2]

- `Timing`: calculate pulse arrival time
    - `ON`: enable timing if true.
    - `Method`: timing method, either `DCFD` or `DIACFD`.
    - `Interpolation`: number of points in one time step after interpolation. Perform pulse interpolation only if value is greater than 1.
    - `Fraction`: CFD fraction, 0 to 1
    - `TimeDelay`: CFD time delay, in ns.
### Plot
- `PlotPulses`: plot pulses
    - `Good pulses`: good pulses
        - `ON`: plot good pulses if true
        - `Number`: number of good pulses to plot

    - `Rejected pulses`: rejected pulses, including pulse rejection, energy cut, PSD cut
        - `ON`: plot rejected pulses if true
        - `Number`: number of rejected pulses to plot

- `Plot Pulse Height Distribution`: plot PHD
    - `ON`: plot and save PHD if true
    - `Calibrated`: use energy calibrated based on pulse height as x-axis if true; use pulse height if false.
    - `xmin`: lower x boundary of the pulse height distribution.
    - `xmax`: upper x boundary of the pulse height distribution. If xmin=xmax=0, automatic bounds will be used.
    - `Bins`: number of bins in pulse height distribution.

- `Plot Pulse Integral Distribution`: plot PID
    - `ON`: plot and save PID if true
    - `Calibrated`: use energy calibrated based on pulse integral as x-axis if true; use pulse integral if false.
    - `xmin`: lower x boundary of the pulse integral distribution.
    - `xmax`: upper x boundary of the pulse integral distribution. If xmin=xmax=0, automatic bounds will be used.
    - `Bins`: number of bins in pulse integral distribution.

- `Plot PSD`: plot PSD
    - `ON`: plot and save PSD if true
    - `Calibrated`: use energy as x-axis if true; use pulse integral if false.
    - `xmin`: lower x boundary of PSD.
    - `xmax`: upper x boundary of PSD. 
    - `XBins`: number of bins along x axis.
    - `ymin`: lower y boundary of PSD
    - `ymax`: upper y boundary of PSD. If xmin=xmax=0 OR ymin=ymax=0, automatic bounds will be used.
    - `YBins`: number of bins along y axis.

### Channel specifics

- `ON`: enable data processing for a channel if true
- `ChannelNumber`: channel number, an integer
- `Path`: path to the binary file

### Coincidence settings

- `ON`: enable coincidence selection if true.
- `CoincidenceChannels`: a pair of channel numbers. Find coincidences between these two channels.
- `TimeWindow`: time window, in ns.
- `SaveTimeDifference`:     save the time difference between each pair of coincident events if true.
- `PlotTOF`
    - `ON`: plot and save the histogram of time differences if true.
    - `xmin`: lower x boundary of ToF.
    - `xmax`: upper x boundary of ToF. 
    - `Bins`: number of histogram bins.

