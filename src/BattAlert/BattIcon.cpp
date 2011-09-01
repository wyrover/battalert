
#include "_Main.h"


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define ICON_WIDTH  16


//---------------------------------------------------------------------------
// BattIcon
//---------------------------------------------------------------------------
BattIcon::BattIcon (void)
{
  // system power status
  m_bHavePowerStatus = false;
  ZeroMemory(&m_SPS, sizeof(m_SPS));

  // load icons
  m_hIconDefault = ::LoadIcon(g_App().GetInstanceHandle(), MAKEINTRESOURCE(IDI_APP));
  m_hIconNoBatt  = ::LoadIcon(g_App().GetInstanceHandle(), MAKEINTRESOURCE(IDI_NOBATT));

  // system tray icon
  {
    int nBmiSize = sizeof(BITMAPINFO);

    m_hDcIcon = CreateCompatibleDC(NULL);
    if (!m_hDcIcon)
      XTHROW EXCODE_SYSTEM_GENERIC, "CreateCompatibleDC() failed ! Error %u : %s", System::LastError(), System::LastErrorString());

    m_pBmiIcon = (BITMAPINFO*)malloc(nBmiSize);
    if (!m_pBmiIcon)
      XTHROW EXCODE_MEMORY, "Failed to allocate BMI (%i bytes) !", nBmiSize);

    ZeroMemory(m_pBmiIcon, nBmiSize);
    m_pBmiIcon->bmiHeader.biSize          = sizeof(m_pBmiIcon->bmiHeader);
    m_pBmiIcon->bmiHeader.biWidth         = ICON_WIDTH;
    m_pBmiIcon->bmiHeader.biHeight        = -ICON_WIDTH;
    m_pBmiIcon->bmiHeader.biPlanes        = 1;
    m_pBmiIcon->bmiHeader.biBitCount      = 24;
    m_pBmiIcon->bmiHeader.biCompression   = BI_RGB;
    m_pBmiIcon->bmiHeader.biSizeImage     = (ICON_WIDTH * ICON_WIDTH * m_pBmiIcon->bmiHeader.biBitCount) / 8;
    m_pBmiIcon->bmiHeader.biXPelsPerMeter = 3200;
    m_pBmiIcon->bmiHeader.biYPelsPerMeter = 3200;
    m_pBmiIcon->bmiHeader.biClrUsed       = 0;
    m_pBmiIcon->bmiHeader.biClrImportant  = 0;

    m_hBmpIcon = CreateDIBSection(m_hDcIcon, m_pBmiIcon, DIB_RGB_COLORS, (PVOID*)&m_pBmpBitsIcon, NULL, 0);
    if (!m_hBmpIcon)
      XTHROW EXCODE_SYSTEM_GENERIC, "CreateDIBSection() failed ! Error %u : %s", System::LastError(), System::LastErrorString());

    m_hIcon = NULL;
  }

  // load normal font
  {
    HRSRC   hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDF_NORMAL), "Fonts");
    DWORD   dwRsrcSize = SizeofResource(NULL, hRsrc);
    HGLOBAL hAddr = LoadResource(NULL, hRsrc);
    DWORD   dwInstalled = 0;

    m_hFontNormalRsrc = AddFontMemResourceEx(hAddr, dwRsrcSize, 0, &dwInstalled);
    if (!m_hFontNormalRsrc)
      XTHROW EXCODE_SYSTEM_GENERIC, "Failed to load normal font resource into memory ! Error (%lu) : %s", System::LastError(), System::LastErrorString());

    m_hFontNormal = CreateFont(
      -8, //-MulDiv(8, GetDeviceCaps(m_hDcIcon, LOGPIXELSY), 72),
      0, 0, 0, FW_NORMAL,
      FALSE, // italic
      FALSE, // underline
      FALSE, // strike out
      ANSI_CHARSET,
      OUT_DEFAULT_PRECIS,
      CLIP_EMBEDDED, //CLIP_DEFAULT_PRECIS,
      NONANTIALIASED_QUALITY, //DEFAULT_QUALITY,
      DEFAULT_PITCH,
      "04b03");
    if (!m_hFontNormal)
      XTHROW EXCODE_SYSTEM_GENERIC, "Failed to load normal font ! Error (%lu) : %s", System::LastError(), System::LastErrorString());
  }

  // load small font
  {
    HRSRC   hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDF_SMALL), "Fonts");
    DWORD   dwRsrcSize = SizeofResource(NULL, hRsrc);
    HGLOBAL hAddr = LoadResource(NULL, hRsrc);
    DWORD   dwInstalled = 0;

    m_hFontSmallRsrc = AddFontMemResourceEx(hAddr, dwRsrcSize, 0, &dwInstalled);
    if (!m_hFontSmallRsrc)
      XTHROW EXCODE_SYSTEM_GENERIC, "Failed to load small font resource into memory ! Error (%lu) : %s", System::LastError(), System::LastErrorString());

    m_hFontSmall = CreateFont(
      -8, //-MulDiv(8, GetDeviceCaps(m_hDcIcon, LOGPIXELSY), 72),
      0, 0, 0, FW_NORMAL,
      FALSE, // italic
      FALSE, // underline
      FALSE, // strike out
      ANSI_CHARSET,
      OUT_DEFAULT_PRECIS,
      CLIP_EMBEDDED, //CLIP_DEFAULT_PRECIS,
      NONANTIALIASED_QUALITY, //DEFAULT_QUALITY,
      DEFAULT_PITCH,
      "04b24");
    if (!m_hFontSmall)
      XTHROW EXCODE_SYSTEM_GENERIC, "Failed to load small font ! Error (%lu) : %s", System::LastError(), System::LastErrorString());
  }
}

//---------------------------------------------------------------------------
// ~BattIcon
//---------------------------------------------------------------------------
BattIcon::~BattIcon (void)
{
  DeleteObject(m_hFontSmall);
  RemoveFontMemResourceEx(m_hFontSmallRsrc);

  DeleteObject(m_hFontNormal);
  RemoveFontMemResourceEx(m_hFontNormalRsrc);

  if (m_hIcon)
    DestroyIcon(m_hIcon);

  DeleteObject(m_hBmpIcon);
  free(m_pBmiIcon);
  DeleteDC(m_hDcIcon);
}

//---------------------------------------------------------------------------
// Refresh
//---------------------------------------------------------------------------
HICON BattIcon::Refresh (void)
{
  int nHours   = 0;
  int nMinutes = 0;
  int nSeconds = 0;

  m_strStatus.Clear();

  // get power status
  m_bHavePowerStatus = true;
  if (!GetSystemPowerStatus(&m_SPS))
  {
    m_bHavePowerStatus = false;
    m_strStatus = "Failed to query power status !";
    return m_hIconDefault;
  }

  // check power status
  if (m_SPS.BatteryFlag & 128) // no system battery
  {
    m_strStatus = "No battery";
    return m_hIconNoBatt;
  }
  if (m_SPS.ACLineStatus != 0 && m_SPS.ACLineStatus != 1) // unknown AC status
  {
    m_strStatus = "Unknown AC status";
    return m_hIconDefault;
  }
  if (m_SPS.BatteryLifePercent > 100) // unknown battery status
  {
    m_strStatus = "Unknown batteru status";
    return m_hIconDefault;
  }

  // prepare status strings
  if (m_SPS.BatteryFlag & 8) // charging
  {
    m_strStatus.Format("Charging (%u%%)", m_SPS.BatteryLifePercent);
  }
  else
  {
    m_strStatus.Format("%u%%", m_SPS.BatteryLifePercent);
    if (m_SPS.BatteryLifeTime != (DWORD)-1)
    {
      nSeconds = (int)m_SPS.BatteryLifeTime;

      if (nSeconds >= 3600)
      {
        nHours   = nSeconds / 3600;
        nSeconds = nSeconds % 3600;
      }
      if (nSeconds >= 60)
      {
        nMinutes = nSeconds / 60;
        nSeconds = nSeconds % 60;
      }

      m_strStatus.FormatAppend(" (%ih%02i)", nHours, nMinutes);
    }
    m_strStatus += " remaining";
  }

  // refresh icon content
  {
    HGDIOBJ hPrevBmp  = SelectObject(m_hDcIcon, m_hBmpIcon);
    HGDIOBJ hPrevFont = SelectObject(m_hDcIcon, m_hFontNormal);
    RECT    rc = { 1, 1, ICON_WIDTH-1, ICON_WIDTH-1 };
    StringA str(int(m_SPS.BatteryLifePercent));

    SetBkMode(m_hDcIcon, TRANSPARENT);
    FillRect(m_hDcIcon, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

    if (m_SPS.BatteryLifePercent > 10)
      SetTextColor(m_hDcIcon, RGB(255, 255, 255));
    else if (m_SPS.BatteryLifePercent > 3)
      SetTextColor(m_hDcIcon, RGB(255, 255, 0));
    else
      SetTextColor(m_hDcIcon, RGB(255, 0, 0));
    DrawText(m_hDcIcon, str.c_str(), str.Length(), &rc, DT_CENTER | DT_VCENTER | DT_NOPREFIX);

    if (m_SPS.BatteryFlag & 8) // charging
    {
      str = "CHR";
      SetTextColor(m_hDcIcon, RGB(0, 255, 255));
    }
    else if (m_SPS.ACLineStatus == 1) // ac power online
    {
      str = "AC";
      SetTextColor(m_hDcIcon, RGB(0, 255, 0));
    }
    else if (m_SPS.BatteryLifeTime != (DWORD)-1)
    {
      str.Format("%i:%02i", nHours, nMinutes);

      if (m_SPS.BatteryLifeTime <= 15*60)
        SetTextColor(m_hDcIcon, RGB(255, 255, 0));
      else if (m_SPS.BatteryLifeTime <= 5*60)
        SetTextColor(m_hDcIcon, RGB(255, 0, 0));
      else
        SetTextColor(m_hDcIcon, RGB(255, 255, 255));
    }
    else
    {
      str.Clear();
    }

    if (str.Length() > 0)
    {
      rc.top += ICON_WIDTH / 2;
      SelectObject(m_hDcIcon, m_hFontSmall);
      DrawText(m_hDcIcon, str.c_str(), str.Length(), &rc, DT_CENTER | DT_VCENTER | DT_NOPREFIX);
    }

    SelectObject(m_hDcIcon, hPrevFont);
    SelectObject(m_hDcIcon, hPrevBmp);
  }

  // convert bitmap to icon
  {
    ICONINFO ii;

    ii.fIcon    = TRUE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask  = m_hBmpIcon;
    ii.hbmColor = m_hBmpIcon;

    if (m_hIcon)
      DestroyIcon(m_hIcon);

    m_hIcon = CreateIconIndirect(&ii);
    if (!m_hIcon)
      XTHROW EXCODE_SYSTEM_GENERIC, "Failed to convert BMP to ICON ! Error (%lu) : %s", System::LastError(), System::LastErrorString());
  }

  XASSERT(m_hIcon);
  return m_hIcon;
}
