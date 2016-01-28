
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TH1.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPaveText.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TList.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <assert.h>

void makePlot(const std::string& inputFilePath, const std::string& canvasName, const std::string& histogram, const std::string& channel, const std::string& xAxisTitle, const std::string& yAxisTitle,
	      const std::string& inputFileName, const std::string& outputFilePath, const std::string& outputFileName)
{
  std::string inputFileName_full = Form("%s%s", inputFilePath.data(), inputFileName.data());
  TFile* inputFile = new TFile(inputFileName_full.data());
  if ( !inputFile ) {
    std::cerr << "Failed to open input file = " << inputFileName_full << " !!" << std::endl;
    assert(0);
  }

  inputFile->ls();

  TCanvas* canvas = dynamic_cast<TCanvas*>(inputFile->Get(canvasName.data()));
  if ( !canvas ) {
    std::cerr << "Failed to load canvas = " << canvasName << " !!" << std::endl;
    assert(0);
  }

  int idxPad = -1;
  if ( histogram == "mVis"    ) idxPad = 1;
  if ( histogram == "mTauTau" ) idxPad = 2;
  if ( !(idxPad >= 1 && idxPad <= 2) ) {
    std::cerr << "Invalid histogram = " << histogram << " !!" << std::endl;
    assert(0);
  }
  TVirtualPad* pad = canvas->GetPad(idxPad);
  std::cout << "pad = " << pad << ": ClassName = " << pad->ClassName() << std::endl;

  TCanvas* canvas_new = new TCanvas("canvas_new", "canvas_new", 900, 850);
  canvas_new->SetFillColor(10);
  canvas_new->SetBorderSize(2);
  canvas_new->SetTopMargin(0.0675);
  canvas_new->SetLeftMargin(0.190);
  canvas_new->SetBottomMargin(0.17);
  canvas_new->SetRightMargin(0.045);
  canvas_new->SetLogx(false);
  canvas_new->SetLogy(false);
  canvas_new->Draw();
  canvas_new->cd();

  //TList* pad_primitives = canvas->GetListOfPrimitives();
  TList* pad_primitives = pad->GetListOfPrimitives();

  TH1* histogramDYJets   = 0;
  TH1* histogramHiggs125 = 0;
  TH1* histogramHiggs200 = 0;
  TH1* histogramHiggs300 = 0;

  TIter pad_nextObj(pad_primitives);
  while ( TObject* obj = pad_nextObj() ) {
    std::string objName = "";
    if ( dynamic_cast<TNamed*>(obj) ) objName = (dynamic_cast<TNamed*>(obj))->GetName();    
    std::cout << "obj = " << obj << ": name = " << objName << ", type = " << obj->ClassName() << std::endl;

    TH1* tmpHistogram = dynamic_cast<TH1*>(obj);
    if ( tmpHistogram ) {
      std::cout << "tmpHistogram:" 
		<< " fillColor = " << tmpHistogram->GetFillColor() << ", fillStyle = " << tmpHistogram->GetFillStyle() << ","
		<< " lineColor = " << tmpHistogram->GetLineColor() << ", lineStyle = " << tmpHistogram->GetLineStyle() << ", lineWidth = " << tmpHistogram->GetLineWidth() << ","
		<< " markerColor = " << tmpHistogram->GetMarkerColor() << ", markerStyle = " << tmpHistogram->GetMarkerStyle() << ", markerSize = " << tmpHistogram->GetMarkerSize() << ","
		<< " integral = " << tmpHistogram->Integral() << std::endl;
      if ( tmpHistogram->GetFillColor() ==   0 ) histogramDYJets   = tmpHistogram;
      if ( tmpHistogram->GetFillColor() == 632 ) histogramHiggs125 = tmpHistogram;
      if ( tmpHistogram->GetFillColor() == 616 ) histogramHiggs200 = tmpHistogram;
      if ( tmpHistogram->GetFillColor() == 600 ) histogramHiggs300 = tmpHistogram;
    }
  }

  if ( !(histogramDYJets && histogramHiggs125 && histogramHiggs200 && histogramHiggs300) ) {
    std::cerr << "Failed to load histograms !!" << std::endl;
    assert(0);
  }

  int lineColors[4] = { 1, 2, 6, 4 };
  int lineStyles[4] = { 1, 1, 1, 1 };
  int lineWidths[4] = { 2, 2, 2, 2 };
  int fillColors[4] = { 10, 2, 6, 4 };
  int fillStyles[4] = { 0, 3002, 3004, 3005 };

  histogramDYJets->SetFillColor(fillColors[0]);
  histogramDYJets->SetFillStyle(fillStyles[0]);
  histogramDYJets->SetLineColor(lineColors[0]);
  histogramDYJets->SetLineStyle(lineStyles[0]);
  histogramDYJets->SetLineWidth(lineWidths[0]);

  histogramHiggs125->SetFillColor(fillColors[1]);
  histogramHiggs125->SetFillStyle(fillStyles[1]);
  histogramHiggs125->SetLineColor(lineColors[1]);
  histogramHiggs125->SetLineStyle(lineStyles[1]);
  histogramHiggs125->SetLineWidth(lineWidths[1]);

  histogramHiggs200->SetFillColor(fillColors[2]);
  histogramHiggs200->SetFillStyle(fillStyles[2]);
  histogramHiggs200->SetLineColor(lineColors[2]);
  histogramHiggs200->SetLineStyle(lineStyles[2]);
  histogramHiggs200->SetLineWidth(lineWidths[2]);

  histogramHiggs300->SetFillColor(fillColors[3]);
  histogramHiggs300->SetFillStyle(fillStyles[3]);
  histogramHiggs300->SetLineColor(lineColors[3]);
  histogramHiggs300->SetLineStyle(lineStyles[3]);
  histogramHiggs300->SetLineWidth(lineWidths[3]);

  TAxis* xAxis = histogramHiggs300->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(1.00);
  xAxis->SetTitleSize(0.080);
  xAxis->SetTitleFont(42);
  xAxis->SetLabelOffset(0.010);
  xAxis->SetLabelSize(0.060);
  xAxis->SetLabelFont(42);
  xAxis->SetTickLength(0.040);
  xAxis->SetNdivisions(505);

  //double xMin = 20.;
  //double xMax = xAxis->GetXmax();
  //xAxis->SetRangeUser(xMin, xMax);

  TAxis* yAxis = histogramHiggs300->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(1.15);
  yAxis->SetTitleSize(0.080);
  yAxis->SetTitleFont(42);
  yAxis->SetLabelOffset(0.010);
  yAxis->SetLabelSize(0.060);
  yAxis->SetLabelFont(42);
  yAxis->SetTickLength(0.040);  
  yAxis->SetNdivisions(505);

  histogramHiggs300->SetTitle("");
  histogramHiggs300->SetStats(false);
  histogramHiggs300->SetMaximum(0.41);
  histogramHiggs300->SetMinimum(0.);
  histogramHiggs300->Draw("hist");
  histogramHiggs200->Draw("histsame");
  histogramHiggs125->Draw("histsame");
  histogramDYJets->Draw("histsame");
  histogramHiggs300->Draw("axissame");

  TLegend* legend_new = new TLegend(0.50, 0.615, 0.87, 0.915, NULL, "brNDC");
  legend_new->SetFillColor(10);
  legend_new->SetFillStyle(0);
  legend_new->SetBorderSize(0);
  legend_new->SetTextFont(42);
  legend_new->SetTextSize(0.055);
  legend_new->SetTextColor(1);
  legend_new->SetMargin(0.20);
  legend_new->AddEntry(histogramDYJets, "Z/#gamma* #rightarrow #tau#tau", "f");
  legend_new->AddEntry(histogramHiggs125, "H(125 GeV) #rightarrow #tau#tau", "f");
  legend_new->AddEntry(histogramHiggs200, "H(200 GeV) #rightarrow #tau#tau", "f");
  legend_new->AddEntry(histogramHiggs300, "H(300 GeV) #rightarrow #tau#tau", "f");
  legend_new->Draw();

  double label_channel_y0;
  if      ( channel == "e#mu"             ) label_channel_y0 = 0.9250;
  else if ( channel == "#mu#tau_{h}"      ) label_channel_y0 = 0.9400;
  else if ( channel == "#tau_{h}#tau_{h}" ) label_channel_y0 = 0.9300;
  else {
    std::cerr << "Invalid channel = " << channel << " !!" << std::endl;
    assert(0);
  }
  TPaveText* label_channel = new TPaveText(0.855, label_channel_y0, 0.945, label_channel_y0 + 0.055, "NDC");
  label_channel->SetFillStyle(0);
  label_channel->SetBorderSize(0);
  label_channel->AddText(channel.data());
  label_channel->SetTextFont(62);
  label_channel->SetTextSize(0.055);
  label_channel->SetTextColor(1);
  label_channel->SetTextAlign(31);
  label_channel->Draw();

  canvas_new->Update();

  std::string outputFileName_full = Form("%s%s", outputFilePath.data(), outputFileName.data());
  size_t idx = outputFileName_full.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName_full, 0, idx);
  canvas_new->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas_new->Print(std::string(outputFileName_plot).append(".root").data());

  delete legend_new;
  delete label_channel;
  delete canvas_new;

  delete inputFile;
}

void makeSVfitMEM_massPlots2_fromBetty()
{
  gROOT->SetBatch(true);

  TH1::AddDirectory(false);

  std::vector<int> massPoints;
  massPoints.push_back(90);
  massPoints.push_back(125);
  massPoints.push_back(200);
  massPoints.push_back(300);
  massPoints.push_back(500);
  massPoints.push_back(800);
  
  std::vector<std::string> histograms;
  histograms.push_back("mVis");
  histograms.push_back("mTauTau");

  std::map<std::string, std::string> xAxisTitles; // key = histogram
  xAxisTitles["mVis"]    = "m_{vis} [GeV]";
  xAxisTitles["mTauTau"] = "m_{#tau#tau} [GeV]";

  std::map<std::string, std::string> yAxisTitles; // key = histogram
  yAxisTitles["mVis"]    = "dN/dm_{vis} [1/GeV]";
  yAxisTitles["mTauTau"] = "dN/dm_{#tau#tau} [1/GeV]";
  
  std::string inputFilePath = "/afs/cern.ch/user/v/veelken/cms/papers/SVfitMEM_NIM_A_git/paper/plots/fromBetty_2016Jan28/";
  std::map<std::string, std::string> inputFileNames_emu; // key = histogram
  inputFileNames_emu["mVis"]       = "plot_9_lin_emu.root";
  inputFileNames_emu["mTauTau"]    = "plot_9_lin_emu.root";
  std::map<std::string, std::string> inputFileNames_muhad; // key = histogram
  inputFileNames_muhad["mVis"]     = "plot_9_lin_muhad.root";
  inputFileNames_muhad["mTauTau"]  = "plot_9_lin_muhad.root";
  std::map<std::string, std::string> inputFileNames_hadhad; // key = mhistogram
  inputFileNames_hadhad["mVis"]    = "plot_9_lin_hadhad.root";
  inputFileNames_hadhad["mTauTau"] = "plot_9_lin_hadhad.root";

  std::string outputFilePath = "/afs/cern.ch/user/v/veelken/cms/papers/SVfitMEM_NIM_A_git/paper/plots/";
  std::map<std::string, std::string> outputFileNames_emu; // key = histogram
  outputFileNames_emu["mVis"]       = "svFitPerformance_emu_visMass.pdf";
  outputFileNames_emu["mTauTau"]    = "svFitPerformance_emu_svFitMass.pdf";
  std::map<std::string, std::string> outputFileNames_muhad; // key = histogram
  outputFileNames_muhad["mVis"]     = "svFitPerformance_muhad_visMass.pdf";
  outputFileNames_muhad["mTauTau"]  = "svFitPerformance_muhad_svFitMass.pdf";
  std::map<std::string, std::string> outputFileNames_hadhad; // key = histogram
  outputFileNames_hadhad["mVis"]    = "svFitPerformance_hadhad_visMass.pdf";
  outputFileNames_hadhad["mTauTau"] = "svFitPerformance_hadhad_svFitMass.pdf";
  
  for ( std::vector<std::string>::const_iterator histogram = histograms.begin();
	histogram != histograms.end(); ++histogram ) {
    std::string xAxisTitle_emu        = xAxisTitles[*histogram];
    std::string yAxisTitle_emu        = yAxisTitles[*histogram];
    std::string inputFileName_emu     = inputFileNames_emu[*histogram];
    std::string outputFileName_emu    = outputFileNames_emu[*histogram];
    makePlot(inputFilePath, "emu", *histogram, "e#mu", xAxisTitle_emu, yAxisTitle_emu, inputFileName_emu, outputFilePath, outputFileName_emu);
    std::string xAxisTitle_muhad      = xAxisTitles[*histogram];
    std::string yAxisTitle_muhad      = yAxisTitles[*histogram];
    std::string inputFileName_muhad   = inputFileNames_muhad[*histogram];
    std::string outputFileName_muhad  = outputFileNames_muhad[*histogram];
    makePlot(inputFilePath, "muhad", *histogram, "#mu#tau_{h}", xAxisTitle_muhad, yAxisTitle_muhad, inputFileName_muhad, outputFilePath, outputFileName_muhad);
    std::string xAxisTitle_hadhad     = xAxisTitles[*histogram];
    std::string yAxisTitle_hadhad     = yAxisTitles[*histogram];
    std::string inputFileName_hadhad  = inputFileNames_hadhad[*histogram];
    std::string outputFileName_hadhad = outputFileNames_hadhad[*histogram];
    makePlot(inputFilePath, "hadhad", *histogram, "#tau_{h}#tau_{h}", xAxisTitle_hadhad, yAxisTitle_hadhad, inputFileName_hadhad, outputFilePath, outputFileName_hadhad);
  }
}
