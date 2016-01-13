
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TMath.h>
#include <TROOT.h>

#include <vector>
#include <iostream>
#include <iomanip>

#include "assert.h"

void showGraph(double canvasSizeX, double canvasSizeY,
	       TGraph* graph, 
	       bool useLogScaleX, double xMin, double xMax, const std::string& xAxisTitle, double xAxisOffset,
	       bool useLogScaleY, double yMin, double yMax, const std::string& yAxisTitle, double yAxisOffset,
	       const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2); 
  canvas->SetTopMargin(0.05);
  canvas->SetLeftMargin(0.19);
  canvas->SetBottomMargin(0.19);
  canvas->SetRightMargin(0.05);
  canvas->SetLogx(useLogScaleX);
  canvas->SetLogy(useLogScaleY);

  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", 10, xMin, xMax);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMaximum(yMax);
  dummyHistogram->Draw("axis");

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(xAxisOffset);
  xAxis->SetTitleSize(0.065);
  xAxis->SetLabelSize(0.055);
  xAxis->SetLabelOffset(0.01);
  xAxis->SetTickLength(0.055);
  xAxis->SetNdivisions(505);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(yAxisOffset);
  yAxis->SetTitleSize(0.070);
  yAxis->SetLabelSize(0.055);
  yAxis->SetLabelOffset(0.01);
  yAxis->SetTickLength(0.055);
  yAxis->SetNdivisions(505);

  graph->SetMarkerColor(1);
  graph->SetLineColor(1);
  graph->Draw("p");

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( useLogScaleY ) outputFileName_plot.append("_log");
  else outputFileName_plot.append("_linear");
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  //canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  //canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
  delete dummyHistogram;
  delete canvas;  
}

void makeSVfitMEM_xSectionPlot()
{
  gROOT->SetBatch(true);

  std::string inputFilePath = "/afs/cern.ch/user/v/veelken/scratch0/SVfitMEM_with_vamp/CMSSW_7_4_6/src/TauAnalysis/SVfitMEM/data/";
  std::string inputFileName = "svFitMEM_xSection_and_AccCorr_13TeV_fitted.root";
  std::string inputFileName_full = Form("%s%s", inputFilePath.data(), inputFileName.data());
  TFile* inputFile = new TFile(inputFileName_full.data());
  if ( !inputFile ) {
    std::cerr << "Failed to open input file = '" << inputFileName_full << "' !!" << std::endl;
    assert(0);
  }
  
  std::string graphName = "graph_Xsection_woAcc_13TeV_hadhad_vamp_fitted";
  TGraph* graph = dynamic_cast<TGraph*>(inputFile->Get(graphName.data()));
  if ( !graph ) {
    std::cerr << "Failed to load graph = '" << graphName << "' from file = '" << inputFileName.data() << "' !!" << std::endl;
    inputFile->ls();
    assert(0);
  }

  std::string outputFileName = "./makeSVfitMEM_xSectionPlot.pdf";
  showGraph(800, 650,
	    graph, 
	    false, 50., 5000., "m_{H} [GeV]", 1.30,
	    true, 1.e-4, 1.e+2, "#sigma(gg #rightarrow H) [pb]", 1.35,
	    outputFileName);

  delete inputFile;
}
