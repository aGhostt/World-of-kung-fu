object MainForm1: TMainForm1
  Left = 101
  Top = 207
  Width = 873
  Height = 540
  Caption = 'CPK Extractor v0.4a'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 233
    Top = 0
    Width = 3
    Height = 467
    Cursor = crHSplit
  end
  object TreeView1: TTreeView
    Left = 0
    Top = 0
    Width = 233
    Height = 467
    Align = alLeft
    Images = ImageList1
    Indent = 19
    ReadOnly = True
    TabOrder = 0
    OnClick = TreeView1Click
  end
  object ListView1: TListView
    Left = 236
    Top = 0
    Width = 629
    Height = 467
    Align = alClient
    Columns = <
      item
        Caption = 'Filename'
        Width = 250
      end
      item
        Caption = 'Filesize'
        Width = 200
      end>
    ReadOnly = True
    RowSelect = True
    PopupMenu = PopupMenu1
    SmallImages = ImageList1
    TabOrder = 1
    ViewStyle = vsReport
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 467
    Width = 865
    Height = 19
    Panels = <
      item
        Width = 200
      end
      item
        Width = 200
      end>
    SimplePanel = False
  end
  object OpenDialog1: TOpenDialog
    Filter = 'CPK Archive|*.cpk'
    Title = 'Select CPK...'
    Left = 16
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 48
    Top = 16
    object File1: TMenuItem
      Caption = 'File'
      object OpenCPK1: TMenuItem
        Caption = 'Open CPK...'
        OnClick = OpenCPK1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About'
        OnClick = About1Click
      end
    end
  end
  object ImageList1: TImageList
    Left = 16
    Top = 48
    Bitmap = {
      494C010102000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000C0C0C000C0C0
      C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0C000C0C0
      C000C0C0C000C0C0C000C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000001C82B5001A80B300177D
      B000157BAE001278AB000F75A8000C72A5000A70A300076DA000056B9E000369
      9C0001679A000066990000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000002187BA0066CCFF001F85B80099FF
      FF006ED4FF006ED4FF006ED4FF006ED4FF006ED4FF006ED4FF006ED4FF006ED4
      FF003AA0D30099FFFF0000669900000000000000000080808000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000248ABD0066CCFF00268CBF0099FF
      FF007AE0FF007AE0FF007AE0FF007AE0FF007AE0FF007AE0FF007AE0FF007AE0
      FF0043A9DC0099FFFF0001679A00000000000000000080808000FFFFFF008080
      8000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000278DC00066CCFF002C92C50099FF
      FF0085EBFF0085EBFF0085EBFF0085EBFF0085EBFF0085EBFF0085EBFF0085EB
      FF004DB3E60099FFFF0003699C00000000000000000080808000FFFFFF008080
      8000FFFFFF00FFFFFF0000008000FFFFFF00FFFFFF000080000000800000FFFF
      FF0000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000298FC20066CCFF003298CB0099FF
      FF0091F7FF0091F7FF0091F7FF0091F7FF0091F7FF0091F7FF0091F7FF0091F7
      FF0056BCEF0099FFFF00056B9E00000000000000000080808000FFFFFF008080
      8000FFFFFF00000080000000FF00FFFFFF00FF000000FFFF0000FF000000FFFF
      FF0000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000002C92C5006ED4FF003399CC0099FF
      FF0099FFFF0099FFFF0099FFFF0099FFFF0099FFFF0099FFFF0099FFFF0099FF
      FF005FC5F80099FFFF00076DA000000000000000000080808000FFFFFF008080
      8000FFFFFF0000FFFF0000008000FFFFFF00FFFFFF00FF000000FFFFFF00FFFF
      FF0000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000002E94C7007AE0FF002C92C500FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080E6FF00FFFFFF000A70A300000000000000000080808000FFFFFF008080
      8000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000003096C90085EBFF0080E6FF002C92
      C5002C92C5002C92C5002C92C5002C92C5002C92C500278DC0002389BC001F85
      B8001B81B4001A80B3001A80B300000000000000000080808000FFFFFF008080
      8000800000008000000080000000800000008000000080000000800000008000
      000000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000003298CB0091F7FF008EF4FF008EF4
      FF008EF4FF008EF4FF008EF4FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00157BAE000000000000000000000000000000000080808000FFFFFF008080
      8000800000000080800080000000800000008000000080000000800000008000
      000000000000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000003399CC00FFFFFF0099FFFF0099FF
      FF0099FFFF0099FFFF00FFFFFF00248ABD002187BA001E84B7001C82B5001A80
      B300177DB0000000000000000000000000000000000080808000FFFFFF008080
      8000808080008080800080808000808080008080800080808000808080008080
      800080808000FFFFFF00C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000003399CC00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00298FC200000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C0C0
      C000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000003399CC003298
      CB003096C9002E94C70000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C0C0
      C000FFFFFF008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C0C0
      C000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFF00000000FFFF800100000000
      8003800100000000000180010000000000018001000000000001800100000000
      0001800100000000000180010000000000018001000000000001800100000000
      0007800100000000000780010000000081FF800300000000C3FF800300000000
      FFFF800300000000FFFF80070000000000000000000000000000000000000000
      000000000000}
  end
  object PopupMenu1: TPopupMenu
    Left = 248
    Top = 24
    object Export1: TMenuItem
      Caption = 'Export'
      OnClick = Export1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Import1: TMenuItem
      Caption = 'Import'
      OnClick = Import1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object Delete1: TMenuItem
      Caption = 'Delete'
      OnClick = Delete1Click
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 48
    Top = 48
  end
  object OpenDialog2: TOpenDialog
    Title = 'Select file'
    Left = 16
    Top = 80
  end
end