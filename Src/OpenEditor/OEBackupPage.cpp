/* 
    Copyright (C) 2002 Aleksey Kochetov

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
#include <COMMON/ExceptionHelper.h>
#include <Common/DirSelectDlg.h>
#include "OpenEditor/OEBackupPage.h"


COEBackupPage::COEBackupPage (SettingsManager& manager)
	: CPropertyPage(COEBackupPage::IDD)
    , m_manager(manager)
{
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

    m_BackupName         = settings.GetFileBackupName().c_str();
    m_BackupDirectory    = settings.GetFileBackupDirectory().c_str();

    switch (settings.GetFileBackup())
    {
    case OpenEditor::ebmNone:
        m_Backup = FALSE;
        m_BackupMethod = -1;
        break;
    case OpenEditor::ebmCurrentDirectory: 
        m_Backup = TRUE;
        m_BackupMethod = 0;
        break;
    case OpenEditor::ebmBackupDirectory:
        m_Backup = TRUE;
        m_BackupMethod = 1;
        break;
    }
}

COEBackupPage::~COEBackupPage()
{
}

void COEBackupPage::DoDataExchange(CDataExchange* pDX)
{
    DDX_Check(pDX, IDC_OE_BACKUP, m_Backup);
    DDX_Text(pDX, IDC_OE_BACKUP_NAME, m_BackupName);
    DDX_Text(pDX, IDC_OE_BACKUP_DIR, m_BackupDirectory);
    DDX_Radio(pDX, IDC_OE_BACKUP_TO_FILE, m_BackupMethod);

    GetDlgItem(IDC_OE_BACKUP_TO_FILE)->EnableWindow(m_Backup);
    GetDlgItem(IDC_OE_BACKUP_TO_DIR)->EnableWindow(m_Backup);
    GetDlgItem(IDC_OE_BACKUP_NAME)->EnableWindow(m_Backup && m_BackupMethod == 0);
    GetDlgItem(IDC_OE_BACKUP_DIR_CHANGE)->EnableWindow(m_Backup && m_BackupMethod == 1);
}


BEGIN_MESSAGE_MAP(COEBackupPage, CPropertyPage)
    ON_BN_CLICKED(IDC_OE_BACKUP,         OnUpdateData)
    ON_BN_CLICKED(IDC_OE_BACKUP_TO_FILE, OnUpdateData)
    ON_BN_CLICKED(IDC_OE_BACKUP_TO_DIR,  OnUpdateData)
    ON_BN_CLICKED(IDC_OE_BACKUP_DIR_CHANGE, OnBnClicked_FileBackupDirChange)
END_MESSAGE_MAP()


// COEBackupPage message handlers

BOOL COEBackupPage::OnApply()
{
    try
    {
        OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
        
        settings.SetFileBackupName((LPCSTR)m_BackupName, false /*notify*/);
        settings.SetFileBackupDirectory((LPCSTR)m_BackupDirectory, false /*notify*/);
        settings.SetFileBackup(m_Backup 
            ? (!m_BackupMethod ? OpenEditor::ebmCurrentDirectory : OpenEditor::ebmBackupDirectory) 
            : OpenEditor::ebmNone,
            false /*notify*/);
    }
    _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void COEBackupPage::OnUpdateData()
{
    UpdateData();
}

void COEBackupPage::OnBnClicked_FileBackupDirChange()
{
    Common::CDirSelectDlg dirDlg("Choose the folder for saving backups:", this, m_BackupDirectory);

    if (dirDlg.DoModal() == IDOK) 
	{
        dirDlg.GetPath(m_BackupDirectory);
        UpdateData(FALSE);
    }

}
