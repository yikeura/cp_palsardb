#include <stdio.h>
#include <string.h>


#define NAME "usr"//ユーザーID
#define PASS "Usr"//ログインパスワード
#define ADD1 150//サーバーIPアドレス
#define ADD2 82
#define ADD3 20
#define ADD4 100
#define INPUT_CSV "test.csv"//読み込みCSV
#define OUTPUT_SH "test.sh"//書き出しシェルスクリプト
#define OUTPUT_DIR "./Copy"//ダウンロードデータの保存先


int main(void)
{
  FILE *file;
  FILE *in;

  int i=0,j=0,k=0,pat,row;
  int len;
  char id[64];

  //ファイルオープン
  in = fopen(INPUT_CSV,"r");
  if(in == NULL)
    {
      printf("read error!!");
      return -1;
    }
  file = fopen(OUTPUT_SH,"w");
  if(file == NULL)
    {
      printf("write error!!");
    }

  //データ数の読み込み
  while (fscanf(in,"%[^,],%03d,%03d",id,&pat,&row) != EOF)
    {
      i++;
    }
  printf("データ数 : %d\n",i-1);
  //ファイル再オープン
  fclose(in);
  in = fopen(INPUT_CSV,"r");
  if(in == NULL)
    {
      printf("read error!!");
      return -1;
    }
  
  //シェルスクリプトの書き出し
  printf("/*データ番号*/ファイル名,path,row\n");
  while (1)
    {  
      if(j==0) fscanf(in,"%[^,],%03d,%03d",id,&pat,&row);
      
      else
	{
	  fscanf(in,"%[^,],%03d,%03d",id,&pat,&row);
	  len = strlen(id);
	  for(k=0;k<len;k++)
	    {
	      id[k] = id[k+1];
	    }
	}
      printf("/*%d*/%s,%03d,%03d\n",j+1,id,pat,row);

      fprintf(file,"expect -c \"\n");
      fprintf(file,"spawn scp -r %s@%d.%d.%d.%d:/gpfs/palsar/DATA/L1/%03d/%03d/%s %s\n",NAME,ADD1,ADD2,ADD3,ADD4,pat,row,id,OUTPUT_DIR);
      fprintf(file,"expect {\n");
      fprintf(file,"\\\"Are you sure you want to continue connecting (yes/no)? \\\" {\n");
      fprintf(file,"send \\\"yes\\r\\\"\n");
      fprintf(file,"expect \\\"password:\\\"\n");
      fprintf(file,"send \\\"%s\\r\\\"\n",PASS);
      fprintf(file,"} \\\"password:\\\" {\n");
      fprintf(file,"send \\\"%s\\r\\\"\n",PASS);
      fprintf(file,"}\n");
      fprintf(file,"}\n");
      fprintf(file,"interact\n");
      fprintf(file,"\"\n");

      j++;
      if(j == i-1) break;
    }

  //ファイルクローズ
  fclose(in);
  fclose(file);
  return 0;
}
