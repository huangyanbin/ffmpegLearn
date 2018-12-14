#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int args,char* argv[])
{
 get_meta_info("../Test.mp4");  
 delete_file("Test");
   find_path_file("./");
    return 0;
    
}
void adts_header(char *szAdtsHeader, int dataLen){

    int audio_object_type = 2;
    int sampling_frequency_index = 7;
    int channel_config = 2;

    int adtsLen = dataLen + 7;

    szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
    szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
    szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
    szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits 
    szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

    szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
    szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits 
    szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
    szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

    szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
    szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
    szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
    szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit  
    szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
    szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

    szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
    szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
    szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
    szAdtsHeader[6] = 0xfc;
}

int get_meta_info(char *path)
{
  
    AVFormatContext *fmt_ctx = NULL;
    AVPacket apt;
    char *desc =  "../Test.acc";
    int res;
    int len;
    int audioIndex;
    av_log_set_level(AV_LOG_INFO);
    av_register_all();
    res = avformat_open_input(&fmt_ctx,path,NULL,NULL);
    if(res <0)
    {
        av_log(NULL,AV_LOG_ERROR,"open fail %s file",path);
        goto __fail;
    }
    av_dump_format(fmt_ctx,0,path,0);
    FILE *file_acc = fopen(desc,"wb");
    if(!file_acc){
         av_log(NULL,AV_LOG_ERROR,"create fail file %s",desc);
    }
    apt.data = NULL;
    apt.stream_index =0;
    av_init_packet(&apt);
    audioIndex = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    while((res = av_read_frame(fmt_ctx,&apt))>=0){
        if(audioIndex == apt.stream_index)
        {
            char head_buffer[7];
            adts_header(head_buffer,7);
            fwrite(head_buffer,1,7,file_acc); 
            len = fwrite(apt.data,1,apt.size,file_acc);
            if(len != apt.size)
            {
                av_log(NULL,AV_LOG_WARNING," write %s file len is no equal data size",desc);
            }
        }
        av_packet_unref(&apt);
    }
    __fail:
    if(file_acc){
        fclose(file_acc);
    }
    avformat_close_input(&fmt_ctx);
    return res;
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