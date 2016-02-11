/*
---------------------------------------------------------
    Author:Mohamed Hassan Helal
    Team:  MOTSL TEAM
    library:frame
    Date: 10/2/2016
---------------------------------------------------------
*/

#include "frame.h"

unsigned char nRF24L01_Freq = 0;
unsigned char nRF24L01_power_rate = 0;
unsigned char rxdata[]={0};


unsigned char decoding (unsigned char rxframe[],unsigned long rxsize){
    unsigned char Type= ( rxframe[0]>>6)&0x03;                         //type 2bit ---> xx......
    unsigned char mode= ( rxframe[0]>>4)&0x03;                         //mode 2bit ---> ..xx....
    unsigned char select = ( rxframe[0])&0x0f;                         //select 4bit -> ....xxxx

    unsigned char data_type=( rxframe[0]>>3)&0x07;                     //data_type 3bit ---> ..xxx...  in case of Data: image,video,text,file...
    unsigned char data_format=( rxframe[0])&0x07;                      //data_formate 3bit-> .....xxx  ex image: JPEG,PNG,JPEG2000,GIF...

    unsigned long data_number=0;
    unsigned char nRF24L01_power_rate = 0;
    unsigned char nRF24L01_Freq = 0;

    switch(Type){
        //----------------------------------------------------------------//
        /*  Data Mode (case 00)   */
              case 0x00:
                      if(rxframe[0]==0)return 0;
                      for(data_number=0;data_number<rxsize;data_number++){
                           rxdata[data_number]=rxframe[data_number+1];
                      }
                      switch(data_type){
                                        case 0x00:                                        //Data is text
                                                  switch(data_format){
                                                                case 0x00: return 1;     //reserved Don't Change it  other type

                                                                case 0x01:               //text
                                                                          return 1;
                                                                case 0x02:
                                                                          return 1;
                                                                default:  return 0;
                                                        }
                                        case 0x01:                                      //Data is Image
                                                  switch(data_format){
                                                                case 0x00:                  //JPEG
                                                                          return 1;
                                                                case 0x01:                  //PNG
                                                                          return 1;
                                                                case 0x02:                  //JPEG 2000
                                                                          return 1;
                                                                case 0x03:                  //BMP
                                                                          return 1;
                                                                case 0x04:                  //Tiff
                                                                          return 1;
                                                                case 0x05:                  //.GIF
                                                                          return 1;
                                                                case 0x06:                  //BPG
                                                                          return 1;
                                                                case 0x07:                  //WEBP
                                                                          return 1;
                                                                default:  return 0;
                                                        }

                                        case 0x02:                                      //Data is Text & data Files
                                                  switch(data_format){
                                                                case 0x00:                  //DOC
                                                                          return 1;
                                                                case 0x01:                  //DOCX
                                                                          return 1;
                                                                case 0x02:                  //RTF
                                                                          return 1;
                                                                case 0x03:                  //WPS
                                                                          return 1;
                                                                case 0x04:                  //PDF
                                                                          return 1;
                                                                case 0x05:                  //txt
                                                                          return 1;
                                                                case 0x06:                  //log  or //alx  simple text
                                                                          return 1;
                                                                case 0x07:                  //.CSV
                                                                          return 1;
                                                                default:  return 0;
                                                        }
                                        case 0x03:                                      //Data is Audio file
                                                  switch(data_format){
                                                                case 0x00:                  //MP3
                                                                          return 1;
                                                                case 0x01:                  //WAV
                                                                          return 1;
                                                                case 0x02:                  //IFF
                                                                          return 1;
                                                                case 0x03:                  //AIF
                                                                          return 1;
                                                                case 0x04:                  //WMA
                                                                          return 1;
                                                                case 0x05:                  //M3U
                                                                          return 1;
                                                                case 0x06:                   //logic   apple audio
                                                                          return 1;
                                                                case 0x07:                  //MPA
                                                                          return 1;
                                                                default:  return 0;
                                                        }
                                        case 0x04:                                      //Data is Video file
                                                  switch(data_format){
                                                                case 0x00:                  //MP4
                                                                          return 1;
                                                                case 0x01:                  //.RM
                                                                          return 1;
                                                                case 0x02:                  //3GP
                                                                          return 1;
                                                                case 0x03:                  //AVI
                                                                          return 1;
                                                                case 0x04:                  //MPG
                                                                          return 1;
                                                                case 0x05:                  //MOV
                                                                          return 1;
                                                                case 0x06:                  //SWF
                                                                          return 1;
                                                                case 0x07:                  //WMV
                                                                          return 1;
                                                                default:  return 0;
                                                        }
                                        case 0x05:                                      //Data is Compressed file & office file
                                                  switch(data_format){
                                                                case 0x00:                  //RAR
                                                                          return 1;
                                                                case 0x01:                  //ZIP
                                                                          return 1;
                                                                case 0x02:                  //7z
                                                                          return 1;
                                                                case 0x03:                  //7Z
                                                                          return 1;
                                                                case 0x04:                  //ppt
                                                                          return 1;
                                                                case 0x05:                  //pptx
                                                                          return 1;
                                                                case 0x06:                  //xls
                                                                          return 1;
                                                                case 0x07:                  //xlr
                                                                          return 1;
                                                                default:  return 0;
                                                        }

                                        case 0x06:                                      //Data is video files & other
                                                  switch(data_format){
                                                                case 0x00:
                                                                          return 1;         ////ogg
                                                                case 0x01:
                                                                          return 1;         ////amr
                                                                case 0x02:
                                                                          return 1;         ////aac
                                                                case 0x03:
                                                                          return 1;         //
                                                                case 0x04:
                                                                          return 1;         //swf flash
                                                                case 0x05:
                                                                          return 1;         //csh photoshop vector graphics
                                                                case 0x06:
                                                                          return 1;         //abr photoshop
                                                                case 0x07:
                                                                          return 1;         //
                                                                default:  return 0;
                                                        }
                                        case 0x07:                                      //web & androide files
                                                  switch(data_format){
                                                                case 0x00:
                                                                          return 1;        //HTML
                                                                case 0x01:
                                                                          return 1;         //crx google chrome
                                                                case 0x02:
                                                                          return 1;         // xpi firefox
                                                                case 0x03:                  //eml
                                                                          return 1;
                                                                case 0x04:                  //ipa apple application
                                                                          return 1;
                                                                case 0x05:                  //apk android
                                                                          return 1;
                                                                case 0x06:                  //.asec android
                                                                          return 1;
                                                                case 0x07:                  //nbu nokia
                                                                          return 1;
                                                                default:  return 0;
                                                        }
                                        default: return 0;
                            }


        //----------------------------------------------------------------//
        /*  Configuration Mode (case 01)   */
            case 0x01: switch(mode){					                   //Configuration (channel/power/Data rate)
                                    case 0x00:                                            //Channel Select
                                                nRF24L01_Freq = rxframe[1];                     //Channel between 0,125                                																					//configure (channel/power/Data rate)
                                                if((nRF24L01_Freq>125)&&(nRF24L01_Freq<0))      //125-->0x7D hex
                                                {return 0;}
                                                else return 1;

                                    case 0x01: 											//Power
                                              switch(select){
                                                            case 0x00:                           				//P0dBm
                                                                      nRF24L01_power_rate|=0x06;
                                                                      return 1;
                                                            case 0x01: 											//Pm6dBm
                                                                      nRF24L01_power_rate|=0x04;
                                                                      return 1;
                                                            case 0x02: 											//Pm12dBm
                                                                      nRF24L01_power_rate|=0x02;
                                                                      return 1;
                                                            case 0x03:      									//Pm18dBm
                                                                      nRF24L01_power_rate|=0x00;
                                                                      return 1;
                                                            default: return 0;
															}
                                    case 0x02: 											//Data rate
                                              switch(select){	    																				//configure (channel/power/Data rate)
                                                            case 0x00:                           				//R2mbps
                                                                      nRF24L01_power_rate|=0x08;
                                                                      return 1;
                                                            case 0x01: 											//R1mbps
                                                                      nRF24L01_power_rate|=0x00;
                                                                      return 1;
                                                            case 0x02: 											//R250kbps
                                                                      nRF24L01_power_rate|=0x20;
                                                                      return 1;
                                                            default: return 0;
															}
                                    case 0x03:
                                              switch(select){													//Sleep / wake up
                                                            case 0x00: 		    								//Sleep

                                                                      return 1;
                                                            case 0x01:											//Wake up

                                                                      return 1;
                                                            //-------------------------/case0x 02 for future use
                                                            case 0x03:                                          //Broad cast ID
                                                                      return 1;
                                                             default:return 0;
															}
                                    default:return 0;
            }
			//----------------------------------------------------------------//
			/*  Control Mode (case 02)   */
            case 0x02:
                      switch(mode){                                          //control mode
                                   case 0x00:															    //Tx Data
                                             TX_Mode();
                                             return 1;
                                   case 0x01:               										 		//Rx single Channel
                                             RX_Mode();
                                             return 1;
                                   case 0x02:                           					                //Rx multi Channel

                                             return 1;
                                   case 0x03:               												//Search

                                             return 1;
                                   default:return 0;
								}
			//----------------------------------------------------------------//
			/*  future use for voice call (case 03)   */


			//----------------------------------------------------------------//
			/*  default return 0   */
			default:return 0;
		}

}



