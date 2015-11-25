
#include <TCanvas.h>
#include <TString.h>
#include <TH1.h>
#include <TAxis.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <TArrow.h>
#include <TEllipse.h>
#include <TBox.h>
#include <TLine.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
#include <TImage.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

int getLineWidth(int lineWidth, int globalScaleFactor)
{
  double lineWidth_scaled = lineWidth*TMath::Power(globalScaleFactor, 0.7);
  return TMath::Nint(lineWidth_scaled);
}

void makeTauDecayParametrizationPlot()
{
  gROOT->SetBatch(true);
  
  int globalScaleFactor = 10;

  double thetaVis        = 118*TMath::Pi()/180;
  double lVis            = 440*globalScaleFactor;
  double dThetaCone      = 24*TMath::Pi()/180;
  double lConeAxis       = 300*globalScaleFactor;
  double lConeLeft       = 325*globalScaleFactor;
  double lConeRight      = 325*globalScaleFactor;
  double lInvis          = 440*globalScaleFactor;
  double thetaInvis      = thetaVis + 15*TMath::Pi()/180;
  double lCoordAxisX     = 220*globalScaleFactor;
  double thetaCoordAxisX = thetaVis - 90*TMath::Pi()/180;
  double lCoordAxisY     = 205*globalScaleFactor;
  double thetaCoordAxisY = thetaVis - 135*TMath::Pi()/180;
  double xApex           = 530*globalScaleFactor;
  double yApex           =  20*globalScaleFactor;

  int    xWidth          = 900*globalScaleFactor;
  int    yWidth          = 460*globalScaleFactor;

  double textSize = 0.06;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, xWidth, yWidth);
  canvas->Range(0, 0, xWidth, yWidth);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  TPad* pad = new TPad("pad", "pad", 0.00, 0.42, 0.82, 1.00, 10, 0, 0);
  pad->SetFillStyle(0);
  pad->Draw("same");
  pad->cd();
 
  canvas->cd();
  
  //-----------------------------------------------------------------------------
  // draw beam axis
  TArrow* beamaxis_arrow = new TArrow(xWidth - 50*globalScaleFactor, yApex, 130*globalScaleFactor, yApex, 0.02, "|>");
  beamaxis_arrow->SetLineColor(1);
  beamaxis_arrow->SetLineWidth(getLineWidth(2, globalScaleFactor));
  beamaxis_arrow->Draw();

  TLatex* beamaxis_text = new TLatex(8*globalScaleFactor, yApex - 7*globalScaleFactor, "beam axis");
  beamaxis_text->SetTextColor(1);
  beamaxis_text->SetTextFont(42);
  beamaxis_text->SetTextSize(textSize);
  beamaxis_text->Draw();
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // draw cone around momentum vector of visible tau decay products
  TEllipse* cone_ellipse = new TEllipse(
    xApex - lConeAxis*TMath::Cos(thetaVis), yApex + lConeAxis*TMath::Sin(thetaVis), 
    1.08*lConeAxis*TMath::Sin(dThetaCone), 1.08*0.33*lConeAxis*TMath::Sin(dThetaCone), 0, 360, 90 - thetaVis*180/TMath::Pi());
  cone_ellipse->SetFillColor(0);
  cone_ellipse->SetFillStyle(0);
  cone_ellipse->SetLineColor(15);
  cone_ellipse->SetLineWidth(getLineWidth(2, globalScaleFactor));
  cone_ellipse->Draw();

  TLine* cone_line1 = new TLine(xApex, yApex, xApex - lConeLeft*TMath::Cos(thetaVis - dThetaCone), yApex + lConeLeft*TMath::Sin(thetaVis - dThetaCone));
  cone_line1->SetLineColor(15);
  cone_line1->SetLineWidth(getLineWidth(2, globalScaleFactor));
  cone_line1->Draw();
  
  TLine* cone_line2 = new TLine(xApex, yApex, xApex - lConeRight*TMath::Cos(thetaVis + dThetaCone), yApex + lConeRight*TMath::Sin(thetaVis + dThetaCone));
  cone_line2->SetLineColor(15);
  cone_line2->SetLineWidth(getLineWidth(2, globalScaleFactor));
  cone_line2->Draw();
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // draw axes of local coordinate system
  double x0CoordAxis = xApex - lConeAxis*TMath::Cos(thetaVis);
  double y0CoordAxis = yApex + lConeAxis*TMath::Sin(thetaVis);

  TArrow* xAxis_arrow = new TArrow(x0CoordAxis, y0CoordAxis, x0CoordAxis - lCoordAxisX*TMath::Cos(thetaCoordAxisX), y0CoordAxis + lCoordAxisX*TMath::Sin(thetaCoordAxisX), 0.01, "|>");
  xAxis_arrow->SetLineColor(12);
  xAxis_arrow->SetLineWidth(getLineWidth(2, globalScaleFactor));
  xAxis_arrow->SetFillColor(12);
  xAxis_arrow->Draw();

  TLatex* xAxis_text = new TLatex(x0CoordAxis - lCoordAxisX*TMath::Cos(thetaCoordAxisX) - 25*globalScaleFactor, y0CoordAxis + lCoordAxisX*TMath::Sin(thetaCoordAxisX) - 6*globalScaleFactor, "e_{x}");
  xAxis_text->SetTextColor(12);
  xAxis_text->SetTextFont(42);
  xAxis_text->SetTextSize(textSize);
  xAxis_text->Draw();

  TArrow* yAxis_arrow = new TArrow(x0CoordAxis, y0CoordAxis, x0CoordAxis - lCoordAxisY*TMath::Cos(thetaCoordAxisY), y0CoordAxis + lCoordAxisY*TMath::Sin(thetaCoordAxisY), 0.01, "|>");
  yAxis_arrow->SetLineColor(12);
  yAxis_arrow->SetLineWidth(getLineWidth(2, globalScaleFactor));
  yAxis_arrow->SetFillColor(12);
  yAxis_arrow->Draw();

  TLatex* yAxis_text = new TLatex(x0CoordAxis - lCoordAxisY*TMath::Cos(thetaCoordAxisY) - 25*globalScaleFactor, y0CoordAxis + lCoordAxisY*TMath::Sin(thetaCoordAxisY) - 6*globalScaleFactor, "e_{y}");
  yAxis_text->SetTextColor(12);
  yAxis_text->SetTextFont(42);
  yAxis_text->SetTextSize(textSize);
  yAxis_text->Draw();
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // draw phiInvis angle
  TLine* phiInvis_line = new TLine(
    xApex - lConeAxis*TMath::Cos(thetaVis), yApex + lConeAxis*TMath::Sin(thetaVis), 
    xApex - (lConeAxis - 26*globalScaleFactor)*TMath::Cos(thetaInvis), yApex + (lConeAxis - 26*globalScaleFactor)*TMath::Sin(thetaInvis));
  phiInvis_line->SetLineColor(12);
  phiInvis_line->SetLineWidth(getLineWidth(2, globalScaleFactor));
  phiInvis_line->Draw();

  TEllipse* phiInvis_ellipse = new TEllipse(
    xApex - lConeAxis*TMath::Cos(thetaVis), yApex + lConeAxis*TMath::Sin(thetaVis), 
    65*globalScaleFactor, 65*globalScaleFactor*0.33, 180 - thetaCoordAxisX, 180 - (thetaCoordAxisX - 124), 90 - thetaVis*180/TMath::Pi());
  phiInvis_ellipse->SetFillColor(46);
  phiInvis_ellipse->SetFillStyle(3002);
  phiInvis_ellipse->SetLineColor(12);
  phiInvis_ellipse->SetLineWidth(getLineWidth(1, globalScaleFactor));
  phiInvis_ellipse->Draw();

  phiInvis_line->Draw();

  //TBox* phiInvis_box = new TBox(
  //  xApex - lConeAxis*TMath::Cos(thetaVis) - 93, yApex + lConeAxis*TMath::Sin(thetaVis) + 58, 
  //  xApex - lConeAxis*TMath::Cos(thetaVis) - 74, yApex + lConeAxis*TMath::Sin(thetaVis) + 82);
  //phiInvis_box->SetFillColor(10);
  //phiInvis_box->SetLineColor(10);
  //phiInvis_box->Draw();

  TLatex* phiInvis_text = new TLatex(xApex - lConeAxis*TMath::Cos(thetaVis) - 40*globalScaleFactor, yApex + lConeAxis*TMath::Sin(thetaVis) + 33*globalScaleFactor, "#phi_{inv}");
  phiInvis_text->SetTextColor(46);
  phiInvis_text->SetTextFont(42);
  phiInvis_text->SetTextSize(textSize);
  phiInvis_text->Draw();
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // draw thetaInvis angle
  //TEllipse* thetaInvis_ellipse = new TEllipse(xApex, yApex, 0.85*120, 120, 44, 59, 0);
  TEllipse* thetaInvis_ellipse = new TEllipse(xApex, yApex, 0.85*120*globalScaleFactor, 120*globalScaleFactor, 72.5, 90, -28);
  thetaInvis_ellipse->SetFillColor(46);
  thetaInvis_ellipse->SetFillStyle(3002);
  thetaInvis_ellipse->SetLineColor(12);
  thetaInvis_ellipse->SetLineWidth(getLineWidth(1, globalScaleFactor));
  thetaInvis_ellipse->Draw();

  TBox* thetaInvis_box = new TBox(
    xApex - 120*globalScaleFactor*TMath::Cos(thetaInvis) - 5*globalScaleFactor, yApex + 120*globalScaleFactor*TMath::Sin(thetaInvis) + 5*globalScaleFactor, 
    xApex - 120*globalScaleFactor*TMath::Cos(thetaInvis) + 19*globalScaleFactor, yApex + 120*globalScaleFactor*TMath::Sin(thetaInvis) + 20*globalScaleFactor);
  thetaInvis_box->SetFillColor(10);
  thetaInvis_box->SetLineColor(10);
  thetaInvis_box->Draw();

  TLatex* thetaInvis_text = new TLatex(xApex - 120*globalScaleFactor*TMath::Cos(thetaInvis) - 15*globalScaleFactor, yApex + 120*globalScaleFactor*TMath::Sin(thetaInvis) + 15*globalScaleFactor, "#theta_{inv}");
  thetaInvis_text->SetTextColor(46);
  thetaInvis_text->SetTextFont(42);
  thetaInvis_text->SetTextSize(textSize);
  thetaInvis_text->Draw();
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // draw momentum vector of visible tau decay products
  TArrow* pVis_arrow = new TArrow(xApex, yApex, xApex - lVis*TMath::Cos(thetaVis), yApex + lVis*TMath::Sin(thetaVis), 0.02, "|>");
  pVis_arrow->SetLineColor(1);
  pVis_arrow->SetLineWidth(getLineWidth(3, globalScaleFactor));
  pVis_arrow->Draw();

  TLatex* pVis_text1 = new TLatex(xApex - TMath::Cos(thetaVis)*lVis + 8*globalScaleFactor, yApex + TMath::Sin(thetaVis)*lVis - 10*globalScaleFactor, "p");
  pVis_text1->SetTextColor(1);
  pVis_text1->SetTextFont(62);
  pVis_text1->SetTextSize(textSize);
  pVis_text1->Draw();

  TLatex* pVis_text2 = new TLatex(xApex - TMath::Cos(thetaVis)*lVis + 21*globalScaleFactor, yApex + TMath::Sin(thetaVis)*lVis - 10*globalScaleFactor, "^{vis} || e_{z}");
  pVis_text2->SetTextColor(1);
  pVis_text2->SetTextFont(42);
  pVis_text2->SetTextSize(textSize);
  pVis_text2->Draw();
  //-----------------------------------------------------------------------------
    
  //-----------------------------------------------------------------------------
  // draw momentum vector of neutrino system
  TArrow* pInvis_arrow = new TArrow(xApex, yApex, xApex - lInvis*TMath::Cos(thetaInvis), yApex + lInvis*TMath::Sin(thetaInvis), 0.02, "|>");
  pInvis_arrow->SetLineColor(1);
  pInvis_arrow->SetLineWidth(getLineWidth(3, globalScaleFactor));
  pInvis_arrow->Draw();

  TLatex* pInvis_text1 = new TLatex(xApex - TMath::Cos(thetaInvis)*lInvis + 10*globalScaleFactor, yApex + TMath::Sin(thetaInvis)*lInvis - 10*globalScaleFactor, "p");
  pInvis_text1->SetTextColor(1);
  pInvis_text1->SetTextFont(62);
  pInvis_text1->SetTextSize(textSize);
  pInvis_text1->Draw();

  TLatex* pInvis_text2 = new TLatex(xApex - TMath::Cos(thetaInvis)*lInvis + 25*globalScaleFactor, yApex + TMath::Sin(thetaInvis)*lInvis - 10*globalScaleFactor, "^{inv}");
  pInvis_text2->SetTextColor(1);
  pInvis_text2->SetTextFont(42);
  pInvis_text2->SetTextSize(textSize);
  pInvis_text2->Draw();
  //-----------------------------------------------------------------------------

  cone_ellipse->Draw();
  cone_line1->Draw();
  cone_line2->Draw();

  //phiInvis_box->Draw();
  phiInvis_text->Draw();

  thetaInvis_box->Draw();
  thetaInvis_text->Draw();

  xAxis_arrow->Draw();
  yAxis_arrow->Draw();

  TArrow* pVis_arrow_top = new TArrow(
    xApex - (lConeAxis - 10*globalScaleFactor)*TMath::Cos(thetaVis), yApex + (lConeAxis - 10*globalScaleFactor)*TMath::Sin(thetaVis), 
    xApex - lVis*TMath::Cos(thetaVis), yApex + lVis*TMath::Sin(thetaVis), 0.02, "|>");
  pVis_arrow_top->SetLineColor(1);
  pVis_arrow_top->SetLineWidth(getLineWidth(3, globalScaleFactor));
  pVis_arrow_top->Draw();

  TArrow* pInvis_arrow_top = new TArrow(
    xApex - (lConeAxis - 10*globalScaleFactor)*TMath::Cos(thetaInvis), yApex + (lConeAxis - 10*globalScaleFactor)*TMath::Sin(thetaInvis), 
    xApex - lInvis*TMath::Cos(thetaInvis), yApex + lInvis*TMath::Sin(thetaInvis), 0.02, "|>");
  pInvis_arrow_top->SetLineColor(1);
  pInvis_arrow_top->SetLineWidth(getLineWidth(3, globalScaleFactor));
  pInvis_arrow_top->Draw();

  TPad* tauCoordDefPad = new TPad("tauCoordDefPad","tauCoordDefPad", 0.02, 0.12, 0.50, 0.84);
  tauCoordDefPad->SetFillColor(0);
  tauCoordDefPad->SetBorderSize(0);
  tauCoordDefPad->Draw();
  tauCoordDefPad->cd();
  TImage* tauCoordDef = TImage::Open("./tauCoordinateSystem.tiff");
  tauCoordDef->Draw();
  canvas->cd();

  canvas->Update();

  canvas->Print("tauDecayParametrization.png");
  canvas->Print("tauDecayParametrization.eps");
  //canvas->Print("tauDecayParametrization.pdf"); // CV: bitmaps cannot be shown when saving as PDF; save in PNG format and then convert to PDF using Preview on Mac
  canvas->Print("tauDecayParametrization.root");

  delete canvas;
}
