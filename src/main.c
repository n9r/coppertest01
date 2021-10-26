#include "main.h"

struct Screen* pScreen = NULL;
struct Window* pWindow = NULL;
struct GfxBase* GfxBase;
extern struct Custom custom;

struct IntuiMessage* pMessage;

static int init(void);
static void close(void);

int main(int argc, char* argv[]) {
  if (init() == 0) {

    ULONG mySignals = 0L;
    struct UCopList* myCopperlist;

    myCopperlist = (struct UCopList*)AllocMem(sizeof(struct UCopList), MEMF_PUBLIC | MEMF_CLEAR);

    SetAPen(pWindow->RPort, 1);
    Move(pWindow->RPort, 80, 50);
    Text(pWindow->RPort, "amiga rulez", 11);

    mySignals = SetSignal(0L, 0L);
    int counter01 = 0, counter256 = 0;

    while (!(mySignals & SIGBREAKF_CTRL_C)) {

      if (counter01 < 32) { counter01++; } else { counter01 = 0; }
      if (counter256 < 256) { counter256++; } else { counter256 = 0; }

      CINIT(myCopperlist, 64);
      for (int i = 0; i < 32; i++) {
        CWAIT(myCopperlist, i * 16 - counter256, 0);
        CMOVE(myCopperlist, custom.color[1], 0x08f1 + (i * 0x0844));
        CMOVE(myCopperlist, custom.color[0], 0x08f1 - ((32 + i) * 0x0844));
      }
      CEND(myCopperlist);

      pScreen->ViewPort.UCopIns = myCopperlist;
      RethinkDisplay();
    }
  }
  close();
  return 0;
}

static int init(void) {
  IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 36L);
  if (IntuitionBase == NULL) {
    return -1;
  }

  GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 34);
  if (NULL == GfxBase) {
    return -1;
  }

  pScreen = (struct Screen*)OpenScreenTags(NULL,
    SA_Left, 0,
    SA_Top, 0,
    SA_Width, 320,
    SA_Height, 256,
    SA_Depth, 4,
    SA_Title, (ULONG)"Screen",
    SA_Quiet, TRUE,
    SA_Colors, NULL,
    SA_ShowTitle, TRUE,
    SA_Type, CUSTOMSCREEN,
    // SA_DisplayID, HIRES,
    SA_DisplayID, PAL_MONITOR_ID,
    SA_Draggable, TRUE,
    TAG_DONE);

  if (pScreen == NULL) {
    return -1;
  }

  pWindow = OpenWindowTags(NULL,
    WA_PubScreen, (ULONG)pScreen,
    WA_Left, 20,
    WA_Top, 10,
    WA_Width, 280,
    WA_Height, 200,
    WA_MinWidth, 10,
    WA_MinHeight, 10,
    WA_CloseGadget, TRUE,
    WA_DepthGadget, TRUE,
    WA_SizeGadget, TRUE,
    WA_Title, (ULONG)"Okno",
    WA_Activate, TRUE,
    WA_DragBar, TRUE,
    WA_GimmeZeroZero, TRUE,
    WA_ReportMouse, TRUE,
    WA_IDCMP, IDCMP_CLOSEWINDOW,
    WA_Flags, WFLG_CLOSEGADGET,
    TAG_END);

  if (pWindow == NULL) {
    return -1;
  }
  return 0;
}

static void close(void) {
  if (pWindow) {
    CloseWindow(pWindow);
  }
  if (pScreen) {
    CloseScreen(pScreen);
  }
  if (IntuitionBase) {
    CloseLibrary((struct Library*)IntuitionBase);
  }
  if (GfxBase) {
    CloseLibrary((struct Library*)GfxBase);
  }
}