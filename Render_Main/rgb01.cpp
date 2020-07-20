#include<cstdio>
int main(int c,char**v){FILE*f=fopen(v[1],"wb");
fwrite("\x89PNG\r\n\x1A\n\0\0\0\rIHDR\0\0\1\0\0\0\1\0\x8\2\0\0\0\xD3\x10?1\0\3\6\6IDATx\1",1,43,f);
for(int i,j=0;j<256;j++)for(i=0*fwrite("\0\1\3\xFE\xFC\0",1,6,f);i<256;i++)fputc(255-j,f),fputc(i,f),fputc(j,f);
fwrite("\xBAY\x96\x63\x38\x32)c\0\0\0\0IEND\xAE\x42`\x82",1,20,f);return fclose(f);}