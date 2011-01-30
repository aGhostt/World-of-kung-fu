//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TMainForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *OpenCPK1;
        TTreeView *TreeView1;
        TListView *ListView1;
        TImageList *ImageList1;
        TSplitter *Splitter1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Export1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TStatusBar *StatusBar1;
        TMenuItem *N1;
        TMenuItem *Exit1;
        TMenuItem *N2;
        TMenuItem *Import1;
        TMenuItem *N3;
        TMenuItem *Delete1;
        TOpenDialog *OpenDialog2;
        void __fastcall OpenCPK1Click(TObject *Sender);
        void __fastcall TreeView1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Export1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall Delete1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Import1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations

        char m_VirtualPath[MAX_PATH];

        bool m_bBusy;

        void __fastcall ExportDiretory(char *Name);      

        __fastcall TMainForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm1 *MainForm1;
//---------------------------------------------------------------------------
#endif
