
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TImage.h>
#include <TPad.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

void divideHistogramByBinWidth(TH1* histogram)
{
  //std::cout << "<divideHistogramByBinWidth>:" << std::endl;
  //std::cout << " histogram: name = " << histogram->GetName() << ", title = " << histogram->GetTitle() << std::endl;
  TAxis* xAxis = histogram->GetXaxis();
  int numBins = xAxis->GetNbins();
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double binContent = histogram->GetBinContent(iBin);
    double binError = histogram->GetBinError(iBin);
    double binWidth = xAxis->GetBinWidth(iBin);
    histogram->SetBinContent(iBin, binContent/binWidth);
    histogram->SetBinError(iBin, binError/binWidth);
    //std::cout << "bin #" << iBin << " (x = " << xAxis->GetBinCenter(iBin) << "): before = " << binContent << " +/- " << binError << "," 
    //	        << " after = " << histogram->GetBinContent(iBin) << " +/- " << histogram->GetBinError(iBin) << std::endl;
  }
}

TH1* getHistogram(TFile* inputFile, const TString& dqmDirectory, const TString& meName)
{  
  TString histogramName = TString(dqmDirectory);
  if ( !histogramName.EndsWith("/") ) histogramName.Append("/");
  histogramName.Append(meName);

  TH1* histogram = (TH1*)inputFile->Get(histogramName.Data());
  if ( !histogram) {
    std::cerr << "Failed to load histogram = " << histogramName << " from file = " << inputFile->GetName() << " !!" << std::endl;
    assert(0);
  }

  if ( histogram && !histogram->GetSumw2N() ) histogram->Sumw2();
  if ( histogram->Integral() > 0. ) histogram->Scale(1./histogram->Integral());
  divideHistogramByBinWidth(histogram);
  
  return histogram;
}

void showHistograms1d(std::vector<TH1*>& histograms, double massPoint, const std::vector<std::string>& legendEntries, 
		      const TString& xAxisTitle, const TString& yAxisTitle,
		      const std::string& outputFileName)
{
  if ( histograms.size() == 0 ) return;
  assert(legendEntries.size() >= histograms.size());

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  canvas->SetTopMargin(0.02);
  canvas->SetLeftMargin(0.21);
  canvas->SetBottomMargin(0.17);
  canvas->SetRightMargin(0.02);
  canvas->cd();

  double yMin = +1.e+6;
  double yMax = -1.e+6;
  for ( std::vector<TH1*>::iterator histogram = histograms.begin();
	histogram != histograms.end(); ++histogram ) {
    int numBinsX = (*histogram)->GetNbinsX();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double y = (*histogram)->GetBinContent(iBinX);
      if ( y < yMin ) yMin = y;
      if ( y > yMax ) yMax = y;
    }
  }
  yMin -= 0.06*(yMax - yMin);
  yMax += 0.06*(yMax - yMin);

  yMin = 0.;
  yMax = 2.29;

  TH1* refHistogram = histograms.front();
  refHistogram->SetStats(false);
  refHistogram->SetTitle("");
  refHistogram->SetMinimum(yMin);
  refHistogram->SetMaximum(yMax);
  
  TAxis* xAxis = refHistogram->GetXaxis();
  xAxis->SetTitle("");
  xAxis->SetTitleOffset(1.15);
  xAxis->SetTitleSize(0.070);
  xAxis->SetLabelSize(0.055);
  xAxis->SetLabelOffset(0.010);
  xAxis->SetTickLength(0.035);
  //xAxis->SetNdivisions(505);

  TAxis* yAxis = refHistogram->GetYaxis();
  yAxis->SetTitle("");
  yAxis->SetTitleOffset(1.20);
  yAxis->SetTitleSize(0.070);
  yAxis->SetLabelSize(0.055);
  yAxis->SetLabelOffset(0.010);
  yAxis->SetTickLength(0.035);

  TImage* xAxisLabel = 0;
  TImage* yAxisLabel = 0;
  if ( xAxisTitle.CompareTo("z_{l}") == 0 ) {
    xAxisLabel = TImage::Open("./xAxisLabel_xLep.tiff");
    yAxisLabel = TImage::Open("./yAxisLabel_xLep.tiff");
  } else if ( xAxisTitle.CompareTo("z_{h}") == 0 ) {
    xAxisLabel = TImage::Open("./xAxisLabel_xHad.tiff");
    yAxisLabel = TImage::Open("./yAxisLabel_xHad.tiff");
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }
  
  int colors[] = { 8, 1, 12 };
  int lineStyles[] = { 1, 7, 4 };
  int numHistograms = histograms.size();
  if ( numHistograms > 3 ) {
    std::cerr << "<showHistograms1d>:" << std::endl;
    std::cerr << "Number of histograms must not exceed 3 !!" << std::endl;
    assert(0);
  }

  double legendX0 = 0.24;
  double legendY0 = 0.78;
  TLegend* legend = 0;
  if ( xAxisTitle.CompareTo("z_{l}") == 0 ) {
    legend = new TLegend(legendX0, legendY0, legendX0 + 0.68, legendY0 + 0.19, "", "brNDC"); 
  } else if ( xAxisTitle.CompareTo("z_{h}") == 0 ) {
    legend = new TLegend(legendX0, legendY0, legendX0 + 0.68, legendY0 + 0.19, "", "brNDC"); 
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetFillColor(10);
  legend->SetTextSize(0.0685);   
  legend->SetTextFont(132);
  legend->SetMargin(0.075);

  for ( int iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    TH1* histogram = histograms[iHistogram];
    histogram->SetLineColor(colors[iHistogram]);
    histogram->SetLineStyle(lineStyles[iHistogram]);
    histogram->SetLineWidth(2);

    std::string drawOption = "hist";
    if ( iHistogram > 0 ) drawOption.append("same");
    histogram->Draw(drawOption.data());

    std::string legendEntry = legendEntries[iHistogram];
    legend->AddEntry(histogram, legendEntry.data(), "l");
  }

  TPad* legendEntryPad1 = 0;
  TImage* legendEntry1 = 0;
  TPad* legendEntryPad2 = 0;
  TImage* legendEntry2 = 0;
  if ( xAxisTitle.CompareTo("z_{l}") == 0 ) {
    legendEntryPad1 = new TPad("legendEntryPad1","legendEntryPad1", legendX0 + 0.04, legendY0 + 0.09, legendX0 + 0.26, legendY0 + 0.15);
    legendEntry1 = TImage::Open("./legendEntry_TAUOLA.tiff");
    legendEntryPad2 = new TPad("legendEntryPad2","legendEntryPad2", legendX0 - 0.025, legendY0 + 0.00, 0.98, legendY0 + 0.095);
    legendEntry2 = TImage::Open("./legendEntry_Lep.tiff");
  } else if ( xAxisTitle.CompareTo("z_{h}") == 0 ) {
    legendEntryPad1 = new TPad("legendEntryPad1","legendEntryPad1", legendX0 + 0.04, legendY0 + 0.09, legendX0 + 0.26, legendY0 + 0.15);
    legendEntry1 = TImage::Open("./legendEntry_TAUOLA.tiff");
    legendEntryPad2 = new TPad("legendEntryPad2","legendEntryPad2", legendX0 - 0.027, legendY0 + 0.00, 0.98, legendY0 + 0.105);
    legendEntry2 = TImage::Open("./legendEntry_Had.tiff");
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }

  legend->Draw();

  TPad* xAxisPad = new TPad("xAxisPad","xAxisPad", 0.89, 0.01, 0.98, 0.07);
  xAxisPad->SetFillColor(0);
  xAxisPad->SetBorderSize(0);
  xAxisPad->Draw();
  xAxisPad->cd();
  xAxisLabel->Draw();
  canvas->cd();

  TPad* yAxisPad = new TPad("yAxisPad","yAxisPad", 0.00, 0.76, 0.14, 0.94);
  yAxisPad->SetFillColor(0);
  yAxisPad->SetBorderSize(0);
  yAxisPad->Draw();
  yAxisPad->cd();
  yAxisLabel->Flip(90);
  yAxisLabel->Draw();
  canvas->cd();
  
  //legendEntryPad1->SetFillColor(0);
  //legendEntryPad1->SetBorderSize(2);
  //legendEntryPad1->Draw();
  //legendEntryPad1->cd();
  //legendEntry1->Draw();
  //canvas->cd();

  legendEntryPad2->SetFillColor(0);
  legendEntryPad2->SetBorderSize(0);
  legendEntryPad2->Draw();
  legendEntryPad2->cd();
  legendEntry2->Draw();
  canvas->cd();

  TLine* legendEntryLine2 = 0;
  if ( xAxisTitle.CompareTo("z_{l}") == 0 ) {
    legendEntryLine2 = new TLine(0.0525, 1.9, 0.10, 1.9);
  } else if ( xAxisTitle.CompareTo("z_{h}") == 0 ) {
    legendEntryLine2 = new TLine(0.0525, 1.9, 0.10, 1.9);
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }
  legendEntryLine2->SetLineColor(histograms[1]->GetLineColor());
  legendEntryLine2->SetLineStyle(histograms[1]->GetLineStyle());
  legendEntryLine2->SetLineWidth(histograms[1]->GetLineWidth());
  legendEntryLine2->Draw();

  histograms[0]->Draw("axissame");

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".eps").data());
  //canvas->Print(std::string(outputFileName_plot).append(".pdf").data()); // CV: bitmaps cannot be shown when saving as PDF; save in PNG format and then convert to PDF using Preview on Mac
  canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
  delete legend;
  delete canvas;
}

void makeVisEnFracPlots()
{
  gROOT->SetBatch(true);

  //std::string inputFilePath = "/data1/veelken/tmp/svFitStudies/";
  std::string inputFilePath = "/afs/cern.ch/user/v/veelken/cms/papers/SVfitMEM_NIM_A_git/paper/plots/macros/";
  std::string inputFileName = Form("%s/studySVfitVisPtCuts_2013Jun17.root", inputFilePath.data());
  TFile* inputFile = new TFile(inputFileName.data());

  std::vector<std::string> directories;
  directories.push_back(std::string("plotsBeforeVisEtaAndPtCuts"));
  //directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg1"));
  //directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg2"));
  //directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg1and2"));
  
  std::map<std::string, std::string> selectionLabels;
  selectionLabels["plotsBeforeVisEtaAndPtCuts"]        = "beforeVisPtCuts";
  //selectionLabels["plotsAfterVisEtaAndPtCutsLeg1"]     = "afterVisPtCutsLeg1";
  //selectionLabels["plotsAfterVisEtaAndPtCutsLeg2"]     = "afterVisPtCutsLeg2";
  //selectionLabels["plotsAfterVisEtaAndPtCutsLeg1and2"] = "afterVisPtCutsLeg1and2";
 
  std::vector<std::string> histogramNames;
  histogramNames.push_back("histogramLeg1X");
  histogramNames.push_back("histogramLeg2X");

  std::map<std::string, std::string> plotLabels;
  plotLabels["histogramLeg1X"] = "X1";
  plotLabels["histogramLeg2X"] = "X2";

  std::map<std::string, std::string> xAxisTitles;
  xAxisTitles["histogramLeg1X"] = "z_{l}";
  xAxisTitles["histogramLeg2X"] = "z_{h}";
  
  std::vector<double> massPoints;
  massPoints.push_back(90.);
/*
  massPoints.push_back(105.);
  massPoints.push_back(110.);
  massPoints.push_back(115.);
  massPoints.push_back(120.);
  massPoints.push_back(125.);
  massPoints.push_back(130.);
  massPoints.push_back(135.);
  massPoints.push_back(140.);
  massPoints.push_back(160.);
  massPoints.push_back(200.);
  massPoints.push_back(250.);
  massPoints.push_back(350.);
  massPoints.push_back(500.);
  massPoints.push_back(700.);
  massPoints.push_back(1000.);
 */
  std::vector<std::string> legendEntries_lep;
  legendEntries_lep.push_back("TAUOLA");
  legendEntries_lep.push_back(" ");

  std::vector<std::string> legendEntries_had;
  legendEntries_had.push_back("TAUOLA");
  legendEntries_had.push_back(" ");
  
  for ( std::vector<double>::const_iterator massPoint = massPoints.begin();
	massPoint != massPoints.end(); ++massPoint ) {

    std::cout << "processing mass-point = " << (*massPoint) << " GeV:" << std::endl;

    for ( std::vector<std::string>::const_iterator directory = directories.begin();
	  directory != directories.end(); ++directory ) {
      for ( std::vector<std::string>::const_iterator histogramName = histogramNames.begin();
	    histogramName != histogramNames.end(); ++histogramName ) {
	TString directory_tauola_full = directory->data();
	if ( !directory_tauola_full.EndsWith("/") ) directory_tauola_full.Append("/");
	directory_tauola_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram_tauola = getHistogram(inputFile, directory_tauola_full, histogramName->data());
	TString directory_toyMCps_full = std::string(*directory).append("_toyMCps").data();
	if ( !directory_toyMCps_full.EndsWith("/") ) directory_toyMCps_full.Append("/");
	directory_toyMCps_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram_toyMCps = getHistogram(inputFile, directory_toyMCps_full, histogramName->data());
	TString directory_toyMCme_full = std::string(*directory).append("_toyMCme").data();
	if ( !directory_toyMCme_full.EndsWith("/") ) directory_toyMCme_full.Append("/");
	directory_toyMCme_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram_toyMCme = getHistogram(inputFile, directory_toyMCme_full, histogramName->data());
	std::vector<TH1*> histograms;
	std::vector<std::string> legendEntries;
	histograms.push_back(histogram_tauola);
	if ( (*histogramName) == "histogramLeg1X" ) {
	  histograms.push_back(histogram_toyMCme);
	  legendEntries = legendEntries_lep;
	}
	if ( (*histogramName) == "histogramLeg2X" ) {
	  histograms.push_back(histogram_toyMCps);
	  legendEntries = legendEntries_had;
	}
	std::string outputFileName = 
	  Form("../../figures/makeSVfitToyMCplots_%s_m%1.0f_%s.png", 
	       plotLabels[*histogramName].data(), *massPoint, selectionLabels[*directory].data());
	showHistograms1d(histograms, *massPoint, legendEntries, xAxisTitles[*histogramName].data(), "#frac{d#Gamma}{dz}", outputFileName);
      }
    }
  }

  delete inputFile;
}


