#include<stdio.h>
#include<iostream>
#include<string.h>
using namespace std;

// merge file1 and file2, and encrypt to out.model 
int merge(char *infile1,char *infile2)
{
    FILE * rfid1 = fopen(infile1, "rb");
    FILE * rfid2 = fopen(infile2, "rb");
    FILE * wfid = fopen("./out.model", "wb");

    if((rfid1 == NULL)||(rfid2 == NULL))
    {
        cout<<"read file error"<<endl;
        return 0;
    }

    fseek(rfid1,0,SEEK_END);
    int num1=ftell(rfid1);
    fseek(rfid1,0,SEEK_SET);
    char c;
    for(int i=0;i<num1;i++)
    {
        c = fgetc(rfid1);
        fputc(c^'x',wfid);
    }

    fseek(rfid2,0,SEEK_END);
    int num2=ftell(rfid2);
    fseek(rfid2,0,SEEK_SET);
    for(int j=0;j<num2;j++)
    {
        c = fgetc(rfid2);
        fputc(c^'x',wfid);
    }

    char snum1[20];
    sprintf(snum1,"%010d",num1);
    cout<<snum1<<endl;
    fputs(snum1,wfid);

    char snum2[20];
    sprintf(snum2,"%010d",num2);
    cout<<snum2<<endl;
    fputs(snum2,wfid);

    fputs("de-encryption",wfid);
    fclose(rfid1);
    fclose(rfid2);
    fclose(wfid);
    return 0;
}
// separate and decrypt the infile to part1 and part2
FILE * separate(char *infile)
{
   FILE * rfid = fopen(infile,"rb");
   FILE * wfid1 = fopen("./part1.bin","wb");
   FILE * wfid2 = fopen("./part2.bin","wb");
   fseek(rfid, -20, SEEK_END);
   char s1[11], s2[11];
   fgets(s1,11,rfid);
   fgets(s2,11,rfid);
   cout<<s1<<endl;
   cout<<s2<<endl;
   int num1,num2;
   sscanf(s1,"%d",&num1);
   sscanf(s2,"%d",&num2);
   cout<<num1<<endl;
   cout<<num2<<endl;
   fseek(rfid,0,SEEK_SET);
   char c;
   for(int i=0;i<num1;i++)
   {
        c = fgetc(rfid);
        fputc(c^'x',wfid1);
   }
   for(int j=0;j<num2;j++)
   {
        c = fgetc(rfid);
        fputc(c^'x',wfid2);
   }
   fclose(rfid);
   fclose(wfid1);
   fclose(wfid2);

}


// encryption the infile
int encryption(char *infile)
{
    FILE * rfid = fopen(infile,"rb");
    if(rfid == NULL)
    {
        cout<<"read file error"<<endl;
        return 0;
    }
    FILE * wfid = fopen("./encrypted.bin","wb");
    char c;
    while((c = fgetc(rfid)) != EOF)
    {
        fputc(c^'x',wfid);
    }
    fputs("de-encryption",wfid);
    fclose(rfid);
    fclose(wfid);
    return 0;
}

// decryption the infile
int decryption(char *infile)
{
    FILE * rfid = fopen(infile,"rb");
    if(rfid == NULL)
    {
        cout<<"read file error"<<endl;
        return 0;
    }
    fseek(rfid,-7,SEEK_END);
    char s[8];
    fgets(s,8,rfid);
    if(0==strcmp(s,"de-encryption"))
    {
        cout<<"Encrypted file"<<endl;
    }

    fseek(rfid,0,SEEK_SET);
    FILE * wfid = fopen("./decryption.bin","wb");
    char c;
    while((c = fgetc(rfid)) != EOF)
    {
        fputc(c^'x',wfid);
    }
    fseek(wfid,-7,SEEK_END);
    for(int i=0;i<7;i++)
    {
        fputc(' ',wfid);
    }
    fclose(rfid);
    fclose(wfid);
    return 0;
}

int main(int argc, char* argv[])
{
    int mode = 0; // 0 encryption, 1 decryption
    std::string input_cfg = "m1.cfg";
    std::string input_weight = "m2.cfg";
    if(argc == 3){
        std::cout << "model encryption" << std::endl;
        input_cfg = argv[1];
        input_weight = argv[2];
        mode = 0;
    }else if(argc == 2){
        std::cout << "model decryption" << std::endl;
        input_cfg = argv[1];
        //input_weight = argv[2];
        mode = 1;
    }else{
        cout << "Usage: " << argv[0] << " input_cfg input_weight" << endl;
        return -1;
    }
    //encryption("../m1.cfg");
    //decryption("./out.bin");
    if(mode == 0){
        merge((char*)input_cfg.c_str(), (char*)input_weight.c_str());
    }else{
        separate("./out.model");
    }
    return 0;
}
