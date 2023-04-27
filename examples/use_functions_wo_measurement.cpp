#include <iostream>
#include <iomanip>
#include <ReadRun.h>

// example how to use functions of ReadRun class in a macro without loading a measurement
// an example how to use the implemented fit functions and other functions in python is given in cosmics-fit.ipynb
void use_functions_wo_measurement() {
	int n_entries = 15;
	double bin_size = 1.8;

	// for this example an array is filled with 10/(1+(x-x0)^2):
	double* test_arr = new double[n_entries];
	double* x = new double[n_entries];
	int x0 = static_cast<int>(n_entries / 2);
	cout << "\noriginal: ";
	for (int i = 0; i < n_entries; i++) {
		x[i] = static_cast<double>(i) * bin_size;
		test_arr[i] = 10. / (1. + static_cast<double>((i - x0) * (i - x0)) * (bin_size * bin_size));
		cout << setprecision(2) << test_arr[i] << "\t";
	}
	auto orig = new TGraph(n_entries, x, test_arr);

	// now the array is smoothed with a gauss kernel (method = 2) with a sigma of 1.2 using ReadRun::SmoothArray()
	ReadRun::SmoothArray(test_arr, n_entries, 1.2, 2, bin_size); // sigma = 1.2, method = 2

	// print content of smoothed array
	cout << "\n\nsmoothed: ";
	for (int i = 0; i < n_entries; i++) cout << setprecision(2) << test_arr[i] << "\t";
	auto smoothed = new TGraph(n_entries, x, test_arr); 
	smoothed->SetLineColor(ReadRun::rcolor(1));
	cout << endl;

	// you can also initialze a dummy of the ReadRun class to use its functions. This also works for non-static functions
	ReadRun dummy_class(0);
	dummy_class.SmoothArray(test_arr, n_entries, 1, 0); // sigma = 1, method = 0 -> average over neigboring +-1 bins
	cout << "\naveraged: ";
	for (int i = 0; i < n_entries; i++) cout << setprecision(2) << test_arr[i] << "\t";
	cout << endl;
	auto more_smoothed = new TGraph(n_entries, x, test_arr);	
	more_smoothed->SetLineColor(4);

	// plot results
	orig->Draw("AC+");
	smoothed->Draw("C same");
	more_smoothed->Draw("C same");

	delete[] test_arr;
	delete[] x;
}