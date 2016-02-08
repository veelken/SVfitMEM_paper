
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

TGraph* readGraph(TFile* inputFile, const std::string& graphName)
{
  TGraph* graph = dynamic_cast<TGraph*>(inputFile->Get(graphName.data()));
  if ( !graph ) {
    std::cerr << "<readGraph>: Failed to load graph = " << graphName << " from file = " << inputFile->GetName() << " !!" << std::endl;
    assert(0);
  }
  return (TGraph*)graph->Clone();
}

void showGraph(double canvasSizeX, double canvasSizeY,
	       TGraph* graph, 
	       double xMin, double xMax, const std::string& xAxisTitle, double xAxisOffset,
	       double yMin, double yMax, bool useLogScaleY, const std::string& yAxisTitle, double yAxisOffset,
	       const std::string& outputFileName)
{
  if ( !graph ) {
    std::cerr << "<showGraphs>: graph = NULL --> skipping creation of output file = " << outputFileName << " !!" << std::endl;
    return;
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  canvas->SetLeftMargin(0.16);
  canvas->SetTopMargin(0.03);
  canvas->SetRightMargin(0.02);
  canvas->SetBottomMargin(0.19);
  canvas->SetLogy(useLogScaleY);
  
  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", 100, xMin, xMax);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMaximum(yMax);

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(xAxisOffset);
  xAxis->SetTitleSize(0.070);
  xAxis->SetLabelSize(0.055);
  xAxis->SetLabelOffset(0.010);
  xAxis->SetTickLength(0.035);
  //xAxis->SetNdivisions(505);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(yAxisOffset);
  yAxis->SetTitleSize(0.070);
  yAxis->SetLabelSize(0.055);
  yAxis->SetLabelOffset(0.010);
  yAxis->SetTickLength(0.035);

  dummyHistogram->Draw("axis");

  graph->SetLineColor(1);
  graph->SetLineWidth(3);
  graph->SetMarkerColor(1);
  graph->SetMarkerStyle(8);
  graph->SetMarkerSize(1);
  graph->Draw("C");

  TPaveText* text = new TPaveText(0.78, 0.47, 0.94, 0.91, "NDC");
  text->SetFillColor(10);
  text->SetBorderSize(0);
  text->AddText("#mu = 1.0");
  text->AddText("#sigma = 0.03");
  text->AddText("x_{1} = 0.97");
  text->AddText("#alpha_{1} = 7");
  text->AddText("x_{2} = 1.03");
  text->AddText("#alpha_{2} = 3.5");
  text->SetTextAlign(12); 
  text->SetTextSize(0.055); 
  text->SetTextFont(42); 
  text->Draw();

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( useLogScaleY ) outputFileName_plot.append("_log");
  else outputFileName_plot.append("_linear");
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".eps").data());
  //canvas->Print(std::string(outputFileName_plot).append(".pdf").data()); // CV: bitmaps cannot be shown when saving as PDF; save in PNG format and then convert to PDF using Preview on Mac
  canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
  delete dummyHistogram;
  delete canvas;  
}

void makeHadTauTransferFuncPlot()
{
  gROOT->SetBatch(true);

  std::string inputFilePath = "/afs/cern.ch/user/v/veelken/scratch0/SVfitMEM_with_vamp/CMSSW_7_4_6/src/TauAnalysis/SVfitMEM/data/";
  std::string inputFileName = Form("%s/hadTauTF_toyMC.root", inputFilePath.data());
  TFile* inputFile = new TFile(inputFileName.data());

  std::string graphName = "fit";

  TGraph* graph = readGraph(inputFile, graphName);

  std::string outputFileName = "tf_tauToHadDecays_pT.pdf";

  showGraph(800, 600,
	    graph, 
	    0., 2., "p_{T}^{vis} / #hat{p}_{T}^{vis}", 1.2,
	    0., 6., false, "W_{h}(p_{T}^{vis} | #hat{p}_{T}^{vis}) [1/GeV]", 1.05,
	    outputFileName);
  showGraph(800, 600,
	    graph, 
	    0., 2., "p_{T}^{vis} / #hat{p}_{T}^{vis}", 1.2,
	    2.e-3, 7.9, true, "W_{h}(p_{T}^{vis} | #hat{p}_{T}^{vis}) [1/GeV]", 1.05,
	    outputFileName);

  delete inputFile;
}


