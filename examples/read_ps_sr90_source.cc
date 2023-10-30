#include <iostream>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <TCanvas.h>

using namespace std;

void read_ps_sr90_source() // main
{
	int which = 3; //select meas

	string path;

	// edit for your fs
	path = "/mnt/c/SHiP/data/";

	switch (which) { //specify folder to run below, ALL bin files in this folder will be used
	case(0): {
		path += "18_Sr_ps/"; //
		break;
	}//
	case(1): {
		path += "16_Sr_nops/"; //
		break;
	}//
	case(2): {
		path += "20_Sr_nops_drilled/"; // first version of casing with drilled hole covered with tape
		break;
	}//
	case(3): {
		path += "21_Sr_ps_drilled/"; //
		break;
	}//
	case(4): {
		path += "22_test_box2/"; // The box used for previous measurements
		break;
	}//
	case(5): {
		path += "23_test_box2_reverse/"; //
		break;
	}//
	case(6): {
		path += "24_test_box1/"; // The other triggerbox
		break;
	}//
	case(7): {
		path += "25_test_box1_reverse/"; //
		break;
	}//
	default: {
		path += "18_Sr_ps/"; // default
		break;
	}
	}

	// initialize class
	ReadRun mymeas(0);

	// read data
	mymeas.ReadFile(path, true);

	// only plot certain channels
	mymeas.plot_active_channels = { 9, 14, 15 };

	//apply baseline correction to ALL waveforms <- NEEDED but slow when not compiled
	//mymeas.SmoothAll(3); // smoothing of waveforms. Caution, will bias results!!
	mymeas.CorrectBaseline(0., 50.);	// use mean from 0 ns to 50 ns

	// print events above a threshold to identify interesting events
	mymeas.FractionEventsAboveThreshold(4, true, true, 100, 150);

	mymeas.FractionEventsAboveThreshold(-4, false, false, 100, 150);
	mymeas.FractionEventsAboveThreshold(-6, false, false, 100, 150);
	mymeas.FractionEventsAboveThreshold(-8, false, false, 100, 150);
	mymeas.FractionEventsAboveThreshold(-12, false, false, 100, 150);

	////plotting

	// plot sums of all events per channel
	mymeas.PlotChannelSums(true);

	// investigate charge spectrum. should see photo electron peaks here
	float intwindowminus = 3.;	// lower integration window in ns rel. to max
	float intwindowplus = 5.;	// upper integration window in ns rel. to max
	float findmaxfrom = 100.;	// assume pulse after trigger arrives between here ...
	float findmaxto = 150.;		// ... and here (depends on trigger delay setting etc., for dark counts the signal is random so we look at the whole recorded time range)

	// plot all charge spectrum of channels
	mymeas.PrintChargeSpectrum(intwindowminus, intwindowplus, findmaxfrom, findmaxto, -1e2, 2.e3, 500, 0, 0, 0);

	mymeas.PrintChargeSpectrumPMT(0, 0, findmaxfrom, findmaxto, -2e1, 1.8e2, 202);
	mymeas.PrintChargeSpectrumPMTthreshold(0, 0, findmaxfrom, findmaxto, -2e1, 1.8e2, 202, 4.);

	// timing of maximum
	mymeas.PrintTimeDist(findmaxfrom, findmaxto, findmaxfrom - 5, findmaxto + 5, 60);

	// plot waveforms of individual events
	//plot range
	double ymin = -5;
	double ymax = 25;

	// plot waveforms for certain events with integration window
	bool printfft = false;
	gROOT->SetBatch(kTRUE); // only write to root file
	for (int i = 1; i < mymeas.nevents; i += static_cast<int>(mymeas.nevents / 50)) {
		mymeas.PrintChargeSpectrumWF(intwindowminus, intwindowplus, findmaxfrom, findmaxto, i, ymin, ymax);
		if (printfft) mymeas.PrintFFTWF(i, 0, .6, 64);
	}
	gROOT->SetBatch(kFALSE);
}