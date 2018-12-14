#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int args,char* argv[])
{
   
   delete_file("Test");
   find_path_file("./");
    return 0;
    
}

int find_path_file(const char* path)
{
    int res;
    AVIODirContext *ctx = NULL;
    AVIODirEntry *entry = NULL;
    res = avio_open_dir(&ctx,path,NULL);
    if(res< 0){
        av_log(NULL,AV_LOG_ERROR,"fail open  %s file %s\n",path,av_err2str(res));
        return -1;
    }
    while(1)
    {
        res = avio_read_dir(ctx,&entry);
        if(res <0){
             av_log(NULL,AV_LOG_ERROR,"fail read child file %s \n",av_err2str(res));
             return -1;
        }
        if(!entry){
            break;
        }
        av_log(NULL,AV_LOG_INFO,"file %s %ld \n",entry->name,entry->size);
    }
}


 int delete_file(const char* fileName)
{ 
    int res;
    res = avpriv_io_delete(fileName);
    if(!res)
    {
        av_log(NULL,AV_LOG_ERROR,"fail delete %s file",fileName);
        return res;
        
    }
    av_log(NULL,AV_LOG_INFO,"suc delete %s file",fileName);
    return res;
}