//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#include <Shlobj.h>
#include <stdio.h>

#include <map>

#pragma hdrstop

#include "Main.h"

#include "minilzo.h"
#include "crc.h"
#include "md5.h"

using namespace std;

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm1 *MainForm1;

#define CPK_FLAG_DIR 3

const char *WATERMARK = " I.L.o.v.e.E.r.i.S.a.w.a.c.h.i.k.a.<3 ";

struct tCPKHeader
{
        char format[4];
        unsigned int version;
        unsigned int tableoffset;
        unsigned int u1;   // 0x7c
        unsigned int itemscount;   // 0xff  current ?
        unsigned int u3;   // 0x00
        unsigned int u4;   // 0x7c
        unsigned int itemscount2;   // 0xff
        unsigned int count; // 5000 ?
        unsigned int delfiles;
        unsigned int filesize;
};

struct tCPKItem
{
        unsigned int uid;
        unsigned int props;
        unsigned int parentuid;
        unsigned int fileoffset;
        unsigned int filepaklen;
        unsigned int filelen;
        unsigned int flag;
};

class CCPK
{
private:
        FILE *m_pFile;

        tCPKHeader m_cpkHeader;

        tCPKItem *m_FileTable;

       // vector<tCPKItem> m_Items;

        map<TTreeNode *,unsigned int> m_DirMap;


        unsigned int m_SelectedDirID;

        char **m_Filenames;

        bool ExpandFileTable( tCPKItem *NewItem , char *szFilename);

        void MakeTree(TTreeNode *ParentNode, unsigned int parentid);
        void ReadDirName( tCPKItem *Item, unsigned int id );
        void ReadFileName( tCPKItem *Item, unsigned int id );

        void ExportSingleFile(char *szOutput, tCPKItem *Item);
        void ExportFiles(char *szOutput, tCPKItem *It);

        bool m_bBusyState;

public:
        CCPK() : m_pFile(NULL), m_FileTable(NULL), m_Filenames(NULL), m_SelectedDirID(0) { }

        void ListFiles(TTreeNode *node, char *szDirName);

        bool OpenCPK(char *szFileName);
        void CloseCPK();

        bool FindHash(unsigned int hash);

        void Import(char *szInputPath, char *szFilename, unsigned int hash);

        void Export(char *szFileName, char *szOutput);
        bool Delete(char *szFileName);

        void ExportDir(char *szDirName, char *szOutput);

        bool GetState() { return m_bBusyState; }

        //bool IsLoaded() { return ( (m_SelectedDirID==0) ? false : true ); }
};

void CCPK::CloseCPK()
{
        if (m_pFile)
        {
                fclose(m_pFile);
                m_pFile = NULL;
        }

        if (m_FileTable)
        {
                delete[] m_FileTable;
                m_FileTable = NULL;
        }

        if (m_Filenames)
        {
                for (unsigned int i=0; i<m_cpkHeader.itemscount2; i++) delete[] m_Filenames[i];
                delete[] m_Filenames;
                m_Filenames = NULL;
        }

       // m_Items.clear();
       m_DirMap.clear();


}

void CCPK::ListFiles(TTreeNode *node, char *szDirName)
{
        if (!m_pFile || !szDirName || m_bBusyState) return;

       // TTreeNode *p;
        m_bBusyState = true;

        m_SelectedDirID = m_DirMap[node];
      //  MainForm1->Caption = SelectedDirUID;

        unsigned int currentfile = 0;
        unsigned int i;
        unsigned int parentid;
        char *currentName = 0;
        tCPKItem *Item;

        MainForm1->ListView1->Items->Clear();

      /*  if (m_SelectedDirID==0)
        {
                while( currentfile < m_cpkHeader.itemscount2 )
                {
                        Item = &m_FileTable[currentfile];

                        if (  Item->parentuid == 0 && (Item->props == 65541 || Item->props == 131073 || Item->props == CPK_FLAG_DIR) )
                        {


                                TListItem *ListItem;
                                ListItem = MainForm1->ListView1->Items->Add();

                                       // MainForm1->Caption = Item->props;
                                        if ((Item->props&0xff) == CPK_FLAG_DIR)
                                        {

                                                ListItem->ImageIndex = 0;
                                                ListItem->SubItems->Add("");
                                                ReadDirName( Item, currentfile);
                                        }
                                        else
                                        {
                                                ReadFileName( Item, currentfile);
                                                ListItem->ImageIndex = 1;
                                                ListItem->SubItems->Add(IntToStr(Item->filelen) );
                                        }

                                ListItem->Caption = m_Filenames[currentfile];


                               // ListItem->SubItems->Add("");
                                //return;
                        }

                        currentfile++;

                        Application->ProcessMessages();

                }

                m_bBusyState = false;
                return;
        }       */



        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if ( (Item->props & 0xff) == CPK_FLAG_DIR && (Item->parentuid==m_SelectedDirID))
                {
                                        TListItem *ListItem;
                                        ListItem = MainForm1->ListView1->Items->Add();
                                        ListItem->Caption = m_Filenames[currentfile];
                                        ListItem->ImageIndex = 0;
                                        ListItem->SubItems->Add("");
                 }

                Application->ProcessMessages();
                currentfile++;
        }


        currentfile = 0;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if ( (Item->props & 0xff) != CPK_FLAG_DIR && (Item->props == 65541 || Item->props == 131073) && (Item->parentuid==m_SelectedDirID))
                {
                        ReadFileName( Item, currentfile);

                        currentName = m_Filenames[currentfile];

                        TListItem *ListItem;
                        ListItem = MainForm1->ListView1->Items->Add();
                        ListItem->Caption = currentName;
                        ListItem->ImageIndex = 1;

                        unsigned int size = Item->filelen/1024;
                        if (size==0) ListItem->SubItems->Add(IntToStr(Item->filelen) + " B" );
                        else
                        ListItem->SubItems->Add(IntToStr(size) + " KB" );

                }

                Application->ProcessMessages();
                currentfile++;
        }

        m_bBusyState = false;
}

void CCPK::ReadDirName( tCPKItem *Item, unsigned int id )
{
        if (Item && m_pFile && m_Filenames)
        {
                if (m_Filenames[id]==NULL)
                {
                        unsigned int len = Item->flag;
                        if (len == 0) len = 80;
                        m_Filenames[id] = new char[len];

                        fseek(m_pFile, Item->fileoffset, SEEK_SET);
                        fread(m_Filenames[id], len, 1, m_pFile);
                }
        }
}

void CCPK::ReadFileName( tCPKItem *Item, unsigned int id )
{
        if (Item && m_pFile && m_Filenames)
        {
                if (m_Filenames[id]==NULL)
                {
                        unsigned int uFlag = Item->props & 0xff;

                        unsigned int len = Item->flag;
                        if (len == 0) len = 80;
                        m_Filenames[id] = new char[len];

                        unsigned int position = 0;

                        if ( uFlag == 1) position = Item->fileoffset + Item->filepaklen;
                        else if ( uFlag == 21 || uFlag == 5) position = Item->fileoffset + Item->filelen;
                        else if ( uFlag == 17) position = Item->fileoffset + Item->filepaklen - 0x4f;
                        else position = Item->fileoffset + Item->filepaklen;

                        fseek(m_pFile, position, SEEK_SET);
                        fread(m_Filenames[id], len, 1, m_pFile);
                }
        }
}

bool CCPK::OpenCPK(char *szFileName)
{
        if (m_pFile) return false;

        m_pFile = fopen(szFileName, "rb+");
        if (!m_pFile) return false;

        unsigned char Header[4] = { 0x52, 0x53, 0x54, 0x1a };

        ZeroMemory( &m_cpkHeader, sizeof(tCPKHeader));

        if (fread( &m_cpkHeader, sizeof(tCPKHeader), 1, m_pFile) != 1)
        {
                MessageBox(0, "Cant read file header!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        if (memcmp(m_cpkHeader.format, Header, 4) != 0)
        {
                MessageBox(0, "This is not a CPK file!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        if (m_cpkHeader.version != 1)
        {
                MessageBox(0, "CPK uknown format version!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        if ( fseek( m_pFile, m_cpkHeader.tableoffset, SEEK_SET) != 0)
        {
                MessageBox(0, "FileTable is out of range!", "Error", MB_OK);
                CloseCPK();
                return false;
        }



        m_FileTable = new(std::nothrow) tCPKItem[m_cpkHeader.itemscount2];
        if (!m_FileTable)
        {
                MessageBox(0, "Cannot allocate memory for FileTable!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        m_Filenames = new(std::nothrow) char*[m_cpkHeader.itemscount2];
        if (!m_Filenames)
        {
                MessageBox(0, "Cannot allocate memory for File Names!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        for (unsigned int i=0; i< m_cpkHeader.itemscount2; i++)
        {
                m_Filenames[i] = NULL;
        }

        if ( fread(m_FileTable, sizeof(tCPKItem), m_cpkHeader.itemscount2, m_pFile) != m_cpkHeader.itemscount2)
        {
                MessageBox(0, "Couldnt load FileTable!", "Error", MB_OK);
                CloseCPK();
                return false;
        }

        //MainForm1->Caption = ftell(m_pFile);

       // m_Items.clear();
        char tmp[255];

        unsigned int currentfile = 0;
        tCPKItem *Item;
        char *currFileName = NULL;

        int DirCount = 0;
        unsigned int position = 0;

        int uFlag = 0;// = Item->props & 0xff;
        int uCompression = 0;// = Item->props >> 16;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];
                uFlag = Item->props & 0xff;

                if ( uFlag == CPK_FLAG_DIR) DirCount++;

              //  sprintf(tmp, "%u, %d, %d, Offset(%d) [%d/%d] %d", Item->uid, Item->props, Item->parentuid, Item->fileoffset, Item->filepaklen, Item->filelen, Item->flag);

             //   MainForm1->Memo1->Lines->Add(tmp);

                currentfile++;
        }

     /*   while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                uFlag = Item->props & 0xff;
                uCompression = Item->props >> 16;

                if ( uFlag == CPK_FLAG_DIR) DirCount++;

                //currFileName = &m_Filenames[currentfile];
                if (Item->flag==0) Item->flag = 80;

                m_Filenames[currentfile] = new char[Item->flag];

                if ( uFlag == 1) position = Item->fileoffset + Item->filepaklen;
                else if ( uFlag == 21 || uFlag == 5) position = Item->fileoffset + Item->filelen;
                else if ( uFlag == 17) position = Item->fileoffset + Item->filepaklen - 0x4f;
                else position = Item->fileoffset + Item->filepaklen;


                if (fseek(m_pFile, position, SEEK_SET)!=0)
                {
                        MessageBox(0, "fseek error.", "Error", MB_OK);
                        CloseCPK();
                        return false;
                }

                fread(m_Filenames[currentfile], Item->flag, 1, m_pFile);

                //if ( (Item->props & 0xff) == CPK_FLAG_DIR)
               // sprintf(tmp, "(dir) %s - %d, %d, %d, Offset(%d) [%d/%d] %d", m_Filenames[currentfile], Item->uid, Item->props, Item->parentuid, Item->fileoffset, Item->filepaklen, Item->filelen, Item->flag);
               // else

                sprintf(tmp, "[%s] %.2X %.2X %d (offset: %d) len: %d/%d", m_Filenames[currentfile], uFlag, uCompression, Item->props, Item->fileoffset, Item->filepaklen, Item->filelen );
              //  sprintf(tmp, "%s - %d, %d, %d, Offset(%d) [%d/%d] %d", m_Filenames[currentfile], Item->uid, Item->props, Item->parentuid, Item->fileoffset, Item->filepaklen, Item->filelen, Item->flag);

             //   MainForm1->Memo1->Lines->Add(tmp);
                currentfile++;
        }          */

        MainForm1->TreeView1->Items->Clear();
        MainForm1->ListView1->Items->Clear();
        
        TTreeNode *ParentNode =  MainForm1->TreeView1->Items->AddFirst(NULL, "[CPK Archive Root]");

        unsigned int ParentID = 0;


        for (int i=0; i< DirCount; i++)
        {
                currentfile = 0;

                while( currentfile < m_cpkHeader.itemscount2 )
                {
                        Item = &m_FileTable[currentfile];

                        uFlag = Item->props & 0xff;
                        if ( uFlag == CPK_FLAG_DIR )
                        {
                                ReadDirName(Item, currentfile);
                               //ParentNode = MainForm1->TreeView1->Items->AddChild(ParentNode, m_Filenames[currentfile]);
                        }

                        currentfile++;
                }
        }

        m_DirMap.clear();
        MakeTree(ParentNode, 0);

        m_bBusyState = false;


       /* for (unsigned int i=0; i< m_cpkHeader.itemscount2; i++ )
        {
                Item = &m_FileTable[i];
                uFlag = Item->props & 0xff;

                if ( uFlag == 0x11 || uFlag == 0x15) MainForm1->Memo1->Lines->Add(Item->filepaklen);

        }   */

        m_SelectedDirID = 0;

       // MainForm1->Caption = DirCount;
        //MainForm1->TreeView1->Items->AddChild(ParentNode, "dir 1");

       // MainForm1->Memo1->Lines->Add(IntToStr(currentfile));
      //  CloseCPK();
        return true;
}
//---------------------------------------------------------------------------
void CCPK::MakeTree(TTreeNode *ParentNode, unsigned int parentid)
{

        unsigned int currentfile = 0;
        tCPKItem *Item;

        TTreeNode *Node = NULL;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                        Item = &m_FileTable[currentfile];

                        if ( (Item->props & 0xff) == CPK_FLAG_DIR )
                        {
                                if ( Item->parentuid == parentid)
                                {
                                       // char str[255];
                                       // sprintf(str, "%d %s", Item->uid, m_Filenames[currentfile]);
                                        //MainForm1->Memo1->Lines->Add(str);
                                        Node = MainForm1->TreeView1->Items->AddChild(ParentNode, m_Filenames[currentfile]);
                                        m_DirMap.insert( pair<TTreeNode*, unsigned int>(Node, Item->uid));
                                        MakeTree(Node, Item->uid);
                                }
                        }

                        currentfile++;
        }
}
//---------------------------------------------------------------------------
bool CCPK::Delete(char *szFileName)
{
        unsigned int currentfile = 0;
        tCPKItem *Item;

        bool bFound = false;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if (Item->parentuid == m_SelectedDirID && (Item->props & 0xff) != CPK_FLAG_DIR)
                {

                                if ( m_Filenames[currentfile] )
                                {
                                        if ( strcmp(szFileName,  m_Filenames[currentfile])==0)
                                        {
                                                bFound = true;
                                                break;
                                        }
                                }

                        if (bFound) break;
                }

                currentfile++;
        }

        if (bFound)
        {
                int flag = (Item->props & 0xff);

                if (flag == 1)  // 0100 0200 compressed
                {
                        Item->props = 131089; // 1100 0200   compressed [deleted]
                        Item->parentuid = 0; // NULL
                        Item->filelen = Item->filepaklen;
                        Item->filepaklen =  Item->filepaklen + Item->flag;
                        Item->flag = 0;
                }
                else
                if (flag == 5) // 0500 0100 uncompressed
                {
                        Item->props = 65557; // 1500 0100   uncompressed [deleted]
                        Item->parentuid = 0; // NULL
                        Item->filepaklen =  Item->filelen + Item->flag;
                        Item->flag = 0;
                }
                else return 0;

                fseek(m_pFile, m_cpkHeader.tableoffset, SEEK_SET);

                if ( fwrite(m_FileTable, sizeof(tCPKItem), m_cpkHeader.itemscount2, m_pFile) != m_cpkHeader.itemscount2)
                {
                        MessageBox(0, "Cannot write filetable!", "Error", MB_OK);
                        return 0;
                }

                fseek(m_pFile, 0, SEEK_SET);

                m_cpkHeader.itemscount--;
                m_cpkHeader.delfiles++;

                if (fwrite( &m_cpkHeader, sizeof(tCPKHeader), 1, m_pFile) != 1)
                {
                        MessageBox(0, "Cannot write file header!", "Error", MB_OK);
                        return 0;
                }

                return true;
        }

        return 0;
}
//---------------------------------------------------------------------------
void CCPK::Export(char *szFileName, char *szOutput)
{
        unsigned int currentfile = 0;
        tCPKItem *Item;

        //TTreeNode *Node = NULL;

        bool bFound = false;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if (Item->parentuid == m_SelectedDirID && (Item->props & 0xff) != CPK_FLAG_DIR)
                {

                                if ( m_Filenames[currentfile] )
                                {
                                        if ( strcmp(szFileName,  m_Filenames[currentfile])==0)
                                        {
                                                bFound = true;
                                                break;
                                        }
                                }

                        if (bFound) break;
                }

                currentfile++;
        }

        if (bFound)
        {
                //MainForm1->Caption = "Found!";
                ExportSingleFile(szOutput, Item);

        }
}
//---------------------------------------------------------------------------
void CCPK::ExportDir(char *szDirName, char *szOutput)
{
        unsigned int currentfile = 0;
        tCPKItem *Item;

        char Path[MAX_PATH];
        //TTreeNode *Node = NULL;

        bool bFound = false;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if (Item->parentuid == m_SelectedDirID && (Item->props & 0xff) == CPK_FLAG_DIR)
                {

                                if ( m_Filenames[currentfile] )
                                {
                                        if ( strcmp(szDirName,  m_Filenames[currentfile])==0)
                                        {
                                                bFound = true;
                                                break;
                                        }
                                }

                        if (bFound) break;
                }

                currentfile++;
        }

        if (bFound)
        {
                sprintf(Path, "%s\\%s", szOutput, szDirName);
                CreateDirectory(Path, NULL);
                ExportFiles(Path, Item);

                MainForm1->StatusBar1->Panels->Items[1]->Text = "";
        }
}
//---------------------------------------------------------------------------
void CCPK::ExportFiles(char *szOutput, tCPKItem *It)
{
        unsigned int currentfile = 0;
        tCPKItem *Item;

        char Path[MAX_PATH];

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if (Item->parentuid == It->uid )
                {

                        if ( (Item->props & 0xff) == CPK_FLAG_DIR )
                        {
                                sprintf(Path, "%s\\%s", szOutput, m_Filenames[currentfile]);
                                CreateDirectory(Path, NULL);

                                MainForm1->StatusBar1->Panels->Items[1]->Text = Path;
                                
                                ExportFiles(Path, Item);
                        }
                        else
                        {
                                ReadFileName(Item, currentfile);
                                sprintf(Path, "%s\\%s", szOutput, m_Filenames[currentfile]);

                                MainForm1->StatusBar1->Panels->Items[1]->Text = Path;

                                ExportSingleFile(Path, Item);
                        }
                }

                currentfile++;
                Application->ProcessMessages();
        }

}
//---------------------------------------------------------------------------
void CCPK::ExportSingleFile(char *szOutput, tCPKItem *Item)
{
                FILE *fp = fopen(szOutput, "wb");
                if (!fp) return;

                fseek(m_pFile, Item->fileoffset, SEEK_SET);

                unsigned int size = 0;

                if ( (Item->props>>16) == 2) size = Item->filepaklen;
                else size = Item->filelen;

                unsigned char *buf = new unsigned char[size];
                fread(buf, size, 1, m_pFile);

                if ( (Item->props>>16) == 2) 
                {
                        unsigned char *outbuf = new unsigned char[ Item->filelen ];
                        unsigned int new_len = Item->filelen;

                        int r = lzo1x_decompress(buf, size, outbuf, (unsigned long*)&new_len,NULL);
                        if (r == LZO_E_OK )
                        {
                                fwrite(outbuf, new_len, 1, fp);
                                delete[] outbuf;
                        }
                        else
                        {
                                delete[] outbuf;
                                MessageBox(0, "Decompression failed!", "Error", MB_OK);
                                fclose(fp);
                        }
                }
                else
                {
                        fwrite(buf, size, 1, fp);
                }

                fclose(fp);
                delete[] buf;
}
//---------------------------------------------------------------------------
bool CCPK::FindHash(unsigned int hash)
{
        unsigned int currentfile = 0;
        tCPKItem *Item;

        //bool bFound = false;

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if ( Item->uid == hash ) return true;

                currentfile++;
        }

        return 0;
}
//---------------------------------------------------------------------------
void CCPK::Import(char *szInputPath, char *szFilename, unsigned int hash)
{
        if ( !szInputPath || !szFilename) return;

        FILE *fp = fopen(szInputPath, "rb");
        if (!fp)
        {
                MessageBox(0, "Cannot open input file!", "Error", MB_OK);
                return;
        }

        bool bCompression = false;
        bool bFound = false;

        MD5 md5;
        unsigned int filesize;
        unsigned int lzosize;
        unsigned int compress_size = 0;
        unsigned int required_size = 0;
        unsigned int data_size = 0;

        unsigned int currentfile = 0;
        tCPKItem *Item;

        md5.update(fp);
        unsigned char *md5_hash = md5.raw_digest();

        fseek(fp, 0, SEEK_END);
        filesize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if ( filesize == 0)
        {
                MessageBox(0, "Input file is empty! Nothing to import?", "Error", MB_OK);
                fclose(fp);
                return;
        }

        lzosize = filesize + (filesize / 16) + 64 + 3;

        unsigned char *Buffer = NULL;
        unsigned char *InBuffer = new(std::nothrow) unsigned char[filesize];
        unsigned char *OutBuffer = new(std::nothrow) unsigned char[lzosize];

        if ( !InBuffer || !OutBuffer)
        {
                MessageBox(0, "Cannot allocate memory for buffers!", "Error", MB_OK);
                fclose(fp);
                if ( InBuffer) delete[] InBuffer;
                if (OutBuffer) delete[] OutBuffer;

                return;
        }

        fread(  InBuffer, filesize, 1, fp);
        fclose(fp);

        int r = lzo1x_1_compress(InBuffer, filesize, OutBuffer, (unsigned long*)&compress_size, wrkmem);
        if (r == LZO_E_OK)
        {

                if (  filesize > compress_size ) // is compression effective ?
                {
                        bCompression = true;
                        data_size = compress_size;
                        Buffer = OutBuffer;

                        delete[] InBuffer; // dont need it anymore
                        InBuffer = NULL;
                }
                else   // compression fail. compressed file is larger than original
                {
                        bCompression = false;
                        data_size = filesize;
                        Buffer = InBuffer;

                        delete[] OutBuffer; // dont need it anymore
                        OutBuffer = NULL;
                }
        }
        else  // ops..! something give a damn. file cant be compressed
        {
                bCompression = false;
                data_size = filesize;
                Buffer = InBuffer;

                delete[] OutBuffer; // dont need it anymore
                OutBuffer = NULL;
        }

        char tmpFilename[255];
        unsigned int fnsize = 0;

        memset(tmpFilename, 0xcc, 255);
        strcpy(tmpFilename, szFilename);
        size_t n = strlen(tmpFilename);
        tmpFilename[n+1] = 0;

        unsigned char *md5offset =  (unsigned char*)tmpFilename + n + 0x2a;
        memcpy( md5offset, md5_hash, 16);

        fnsize = n + 0x2a + 16;

        required_size = data_size + fnsize;   // REQUIRED SIZE FOR IMPORT FILE

        while( currentfile < m_cpkHeader.itemscount2 )
        {
                Item = &m_FileTable[currentfile];

                if ( Item->props == 65557 || Item->props == 131089)
                {
                        if ( Item->filepaklen >= required_size)
                        {
                                bFound = true;
                                break;
                        }
                }

                currentfile++;
        }

        if ( bFound )  // found a place to put new file
        {
                tCPKItem NewItem;
                NewItem.uid = hash;
                NewItem.parentuid = m_SelectedDirID;
                NewItem.fileoffset = Item->fileoffset;
                NewItem.flag = fnsize - 1;

                if ( bCompression )
                {
                        NewItem.props = 131073; // 0100 0200
                        NewItem.filepaklen = compress_size;
                        NewItem.filelen = filesize;
                }
                else
                {
                        NewItem.props = 65541; // 0500 0100
                        NewItem.filepaklen = filesize;
                        NewItem.filelen = filesize;
                }

                Item->fileoffset = Item->fileoffset +  required_size;
                Item->filepaklen = Item->filepaklen - required_size;

                ExpandFileTable( &NewItem , szFilename);

                fseek(m_pFile, NewItem.fileoffset, SEEK_SET);
                fwrite(Buffer, data_size, 1, m_pFile);
                fwrite(tmpFilename, fnsize, 1, m_pFile);

                fseek(m_pFile, m_cpkHeader.tableoffset, SEEK_SET);
                fwrite( m_FileTable, sizeof(tCPKItem), m_cpkHeader.itemscount2, m_pFile);// != m_cpkHeader.itemscount2)

                fseek(m_pFile, 0, SEEK_SET);
                fwrite( &m_cpkHeader, sizeof(tCPKHeader), 1, m_pFile);

                TListItem *ListItem;
                ListItem = MainForm1->ListView1->Items->Add();
                ListItem->Caption = szFilename;
                ListItem->ImageIndex = 1;
                unsigned int size = NewItem.filelen/1024;
                if (size==0) ListItem->SubItems->Add(IntToStr(NewItem.filelen) + " B" );
                else
                ListItem->SubItems->Add(IntToStr(size) + " KB" );
                
        }
        else  // we need to create place to put new file
        {
                tCPKItem NewItem;
                NewItem.uid = hash;
                NewItem.parentuid = m_SelectedDirID;
                NewItem.fileoffset = m_cpkHeader.tableoffset;  // here we can put new file
                NewItem.flag = fnsize - 1;

                if ( bCompression )
                {
                        NewItem.props = 131073; // 0100 0200
                        NewItem.filepaklen = compress_size;
                        NewItem.filelen = filesize;
                }
                else
                {
                        NewItem.props = 65541; // 0500 0100
                        NewItem.filepaklen = filesize;
                        NewItem.filelen = filesize;
                }

                ExpandFileTable( &NewItem , szFilename);

                fseek(m_pFile, NewItem.fileoffset, SEEK_SET);
                fwrite(Buffer, data_size, 1, m_pFile);
                fwrite(tmpFilename, fnsize, 1, m_pFile);

                unsigned int newtableoffset = (unsigned int)ftell( m_pFile );
                unsigned int newsize = 0;

                //fseek(m_pFile, m_cpkHeader.tableoffset, SEEK_SET);
                fwrite( m_FileTable, sizeof(tCPKItem), m_cpkHeader.itemscount2, m_pFile);// != m_cpkHeader.itemscount2)



                fseek(m_pFile, 0, SEEK_END);
                newsize = (unsigned int)ftell( m_pFile );

                m_cpkHeader.tableoffset = newtableoffset;
                m_cpkHeader.filesize = newsize;

                fseek(m_pFile, 0, SEEK_SET);
                fwrite( &m_cpkHeader, sizeof(tCPKHeader), 1, m_pFile);

                TListItem *ListItem;
                ListItem = MainForm1->ListView1->Items->Add();
                ListItem->Caption = szFilename;
                ListItem->ImageIndex = 1;
                unsigned int size = NewItem.filelen/1024;
                if (size==0) ListItem->SubItems->Add(IntToStr(NewItem.filelen) + " B" );
                else
                ListItem->SubItems->Add(IntToStr(size) + " KB" );
        }

        delete[] Buffer;
//        delete[] OutBuffer;

}
//---------------------------------------------------------------------------
bool CCPK::ExpandFileTable( tCPKItem *NewItem , char *szFilename)
{
        tCPKItem *Item, *ItemOut;
        tCPKItem *NewFileTable = NULL;
        char **NewFilenames = NULL;
        bool bUpdated = false;

        NewFileTable = new(std::nothrow) tCPKItem[m_cpkHeader.itemscount2 + 1];
        if (!NewFileTable)
        {
                return false;
        }

        NewFilenames = new(std::nothrow) char*[m_cpkHeader.itemscount2 + 1];
        if (!NewFilenames)
        {
                return false;
        }

        unsigned int j=0;
        size_t n;

        for (unsigned int i=0; i< m_cpkHeader.itemscount2 + 1; i++)
        {
                ItemOut = &NewFileTable[i];
                Item = &m_FileTable[j];


                if ( (!bUpdated) && (Item->uid > NewItem->uid) )
                {
                        bUpdated = true;
                        memcpy(ItemOut, NewItem, sizeof(tCPKItem));

                        n = strlen(szFilename);

                        NewFilenames[i] = new char[n+1];
                        memset(NewFilenames[i], 0, n+1);
                        strcpy(NewFilenames[i],  szFilename);
                }
                else
                {
                        memcpy(ItemOut, Item, sizeof(tCPKItem));

                        if (m_Filenames[j]==NULL)
                        {
                                NewFilenames[i] = NULL;
                        }
                        else
                        {
                                n = strlen(m_Filenames[j]);

                                NewFilenames[i] = new char[n+1];
                                memset(NewFilenames[i], 0, n+1);
                                strcpy(NewFilenames[i],  m_Filenames[j]);
                        }

                        j++;
                }
        }

       // FILE *fp = fopen("c:\\DEBUG", "wb");
       // fwrite( m_FileTable, sizeof(tCPKItem), m_cpkHeader.itemscount2 + 1, fp);
       // fclose(fp);

        if (m_FileTable)
        {
                delete[] m_FileTable;
                m_FileTable = NewFileTable;
        }

        if (m_Filenames)
        {
                for (unsigned int i=0; i<m_cpkHeader.itemscount2; i++) delete[] m_Filenames[i];
                delete[] m_Filenames;
                m_Filenames = NewFilenames;
        }



        m_cpkHeader.itemscount++;
        m_cpkHeader.itemscount2++;

}
//---------------------------------------------------------------------------
CCPK g_Cpk;
//---------------------------------------------------------------------------
__fastcall TMainForm1::TMainForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::OpenCPK1Click(TObject *Sender)
{
        if (OpenDialog1->Execute())
        {
              StatusBar1->Panels->Items[1]->Text = "Please wait...";
              Application->ProcessMessages();

                g_Cpk.CloseCPK();


                g_Cpk.OpenCPK( OpenDialog1->FileName.c_str() );

                StatusBar1->Panels->Items[1]->Text = "";
              //  Caption = "done";
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::TreeView1Click(TObject *Sender)
{
        TTreeNode * n = TreeView1->Selected;

        if (m_bBusy) return;

        if (n)
        {
              TTreeNode * p = n;

              char path[MAX_PATH];
              char *pos = m_VirtualPath + MAX_PATH-1;
              char *str;
              size_t len;
              memset(m_VirtualPath, 0, MAX_PATH);

              do
              {
                pos--;
                str = p->Text.c_str();
                len = strlen(str);
                pos = pos - len ;

                memcpy(pos, str, len);
                memcpy(pos+len, "\\", 1);
              }
              while ( (p=p->Parent) );

              char *vp = strchr(pos, '\\');
              if (vp)
              {
                strcpy(m_VirtualPath, vp+1);
              }

             // Caption =  m_VirtualPath;

              m_bBusy = true;

              StatusBar1->Panels->Items[0]->Text = m_VirtualPath;

              StatusBar1->Panels->Items[1]->Text = "Please wait...";
              Application->ProcessMessages();
              g_Cpk.ListFiles( n, n->Text.c_str() );
              StatusBar1->Panels->Items[1]->Text = "";

              m_bBusy = false;
        }


}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        g_Cpk.CloseCPK();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::Export1Click(TObject *Sender)
{
        if (ListView1->ItemIndex < 0) return;

        TListItem *ls = ListView1->Selected;
        if (ls)
        {

                ListView1->Enabled = false;
                TreeView1->Enabled = false;

                if ( ls->ImageIndex == 1)
                {
                        SaveDialog1->FileName = ls->Caption;

                        if (SaveDialog1->Execute())
                        {
                                g_Cpk.Export( ls->Caption.c_str(), SaveDialog1->FileName.c_str() );
                        }
                }
                else
                {
                        ExportDiretory(ls->Caption.c_str());
                }

                ListView1->Enabled = true;
                TreeView1->Enabled = true;
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::About1Click(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::ExportDiretory(char *Name)
{
        char path[MAX_PATH];

        BROWSEINFO lp;

        ZeroMemory(&lp, sizeof(BROWSEINFO));
        lp.hwndOwner = Handle;
                        //lp.lpszTitle = "Select Destination";
                        //lp.pszDisplayName = path;

        LPITEMIDLIST pidl = SHBrowseForFolder(&lp);
        if ( pidl != 0)
        {
                if ( SHGetPathFromIDList ( pidl, path ) )
                {
                        g_Cpk.ExportDir(Name, path);

                        IMalloc * imalloc = 0;
                        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
                        {
                                imalloc->Free ( pidl );
                                imalloc->Release ( );
                        }
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::Exit1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::Delete1Click(TObject *Sender)
{
        if (ListView1->ItemIndex < 0) return;

        TListItem *ls = ListView1->Selected;
        if (ls)
        {

              //  ListView1->Enabled = false;
               // TreeView1->Enabled = false;

                if ( ls->ImageIndex == 1)
                {
                        if (MessageBox(Handle, "Are you sure you want to delete this file?", "Confirm", MB_ICONWARNING|MB_YESNO)==ID_YES)
                        {


                                if ( g_Cpk.Delete( ls->Caption.c_str() ) )
                                {
                                        ls->Delete();
                                }
                        }
                }
                else
                {
                        MessageBox(Handle, "You cannot delete folders!", "Error", MB_ICONWARNING|MB_OK);
                }

               // ListView1->Enabled = true;
                //TreeView1->Enabled = true;
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::FormCreate(TObject *Sender)
{
        lzo_init();
        m_bBusy = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm1::Import1Click(TObject *Sender)
{
        if (TreeView1->Selected)
        {
                if ( OpenDialog2->Execute() )
                {
                        char path[MAX_PATH];
                        unsigned int hash = 0;

                        String filename = ExtractFileName( OpenDialog2->FileName );
                        sprintf(path, "%s%s", m_VirtualPath, filename.c_str());
                        hash = CRC_String(path);

                        if ( g_Cpk.FindHash(hash) )
                        {

                                MessageBox(Handle, "There is already file with that name in this folder. Delete old one before importing new one.", "Error", MB_ICONWARNING|MB_OK);
                                return;
                        }

                        Caption = path;

                        g_Cpk.Import( OpenDialog2->FileName.c_str(), filename.c_str(), hash );


                }

        }
}
//---------------------------------------------------------------------------

