/* 
    Copyright (C) 2004 Edwig Huisman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "stdafx.h"
#include "AboutDlg.h"

// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
          :CDialog(CAboutDlg::IDD, pParent)
{
  m_license = "ODBC Query Tool 1.4.3 Build: 184"
              "\r\nCopyright (c) 2006-2016 Edwig Huisman" 
              "\r\n\r\nThis program is free software; you can redistribute it" 
              "\r\n\r\nThis program is distributed in the hope that it will be useful,"
              " but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY"
              " or FITNESS FOR A PARTICULAR PURPOSE. See the license file for more details."
              "\r\n\r\nCredits go to the following persons:"
              "\r\n\r\nFor the OpenEditor: Aleksey Kochetov"
              "\r\nFor his superb data grid: Chris Maunder"
              "\r\nFor the ODBCRecordset: B.P. Nebbeling"
              "\r\nFor all the testing: Vincent Heuveling"
              "\r\nFor editable querygrids: Marco Brattinga";
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_OEA_LICENSE, m_license);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_OEA_LOGO, OnStnClickedOeaLogo)
END_MESSAGE_MAP()


// CAboutDlg message handlers

void CAboutDlg::OnStnClickedOeaLogo()
{
	// TODO: Add your control notification handler code here
  AfxMessageBox("OK, This is the logo");
}
