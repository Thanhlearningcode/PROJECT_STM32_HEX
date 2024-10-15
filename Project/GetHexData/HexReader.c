#include "HexReader.h"

uint8_t HexData[1024]; // Định nghĩa mảng lưu data
uint32_t CurrentNumberData = 0; // Định nghĩa số lượng data hiện tại
char LineData[50]; // Định nghĩa mảng chứa giá trị đọc được từ file hex
int LineHexData[50]; // Định nghĩa mảng chứa giá trị đọc từ file hex đã chuyển qua dạng hex
HEXFormData hexStruct; // Định nghĩa cấu trúc hexStruct

 /******************************************************************************************
 * @brief   Hàm này lấy phần Data từ Hex file và gán vào 1 mảng
 * @details Hàm này kiểm tra số lượng data của mỗi dòng hex,
 *          sau đó kiểm tra loại record type là gì
 *          nếu record type là 00 thì thực hiện gán các giá trị data vào mảng
 * @param   *fLineData Con trỏ tới mảng chứa data
 * @param   *fp Con trỏ tới file 
 * @return  uint8_t trả về 0 sau khi hoàn thành
 *******************************************************************************************/
uint8_t GetHexData(FILE *fp, char *fLineData){
    HEXField State;
    uint32_t Count = 0;
    uint32_t DataCount = 0;
    while ((ReadLine(fp,fLineData)))
        {
            State = BYTE_COUNT; // gán state về Byte_Count
            for ( Count = 1; Count < strlen(fLineData); Count++) // Lặp tới khi hết 1 dòng HEX
            {   
                switch (State)
                {
                case BYTE_COUNT:
                        ConvertStringToHex(&fLineData[Count],2); // Chuyển đổi 2 phần tử tiếp theo của dòng hex từ string sang hex
                        hexStruct.Byte_Count = LineHexData[0]; // gán giá trị cho Byte_count
                        Count=6; // Gán Count = 6
                        State = RECORD_TYPE;// chuyển tới Case tiếp theo
                        break;
                case RECORD_TYPE:
                        ConvertStringToHex(&fLineData[Count],2); // Chuyển đổi 2 phần tử tiếp theo của dòng hex từ string sang hex
                        hexStruct.Record_Type = LineHexData[0]; // gán giá trị cho Record_type
                        Count=8;// Gán Count = 8
                        State = DATA;// chuyển tới Case tiếp theo
                        break;  
                case DATA:
                        ConvertStringToHex(&fLineData[Count],hexStruct.Byte_Count*2);
                        // printf("%d",hexStruct.Byte_Count);
                        // printf("%d",ConvertHexToDecimal(hexStruct.Byte_Count));
                        for (DataCount = 0; DataCount < hexStruct.Byte_Count; DataCount++)
                        {   
                            if (hexStruct.Record_Type == 0x00) {
                                if (CurrentNumberData < sizeof(HexData)) { // Đảm bảo không tràn mảng
                                HexData[CurrentNumberData] = LineHexData[DataCount]; // Lưu dữ liệu
                                CurrentNumberData++;
                                }
                            }
                        }
                        Count += ConvertHexToDecimal(hexStruct.Byte_Count) * 2 + 4;
                        break;        
                default:
                        return 0;
                }
            }   
        }
        return 0;
}
 /******************************************************************************************
 * @brief   Hàm này lấy giá trị của một dòng hex trong file HEX rồi gán vào mảng data
 * @details Hàm này lấy giá trị của từng phần tử trên 1 dòng rồi gán vào mảng data
 *          lặp lại cho tới khi kết thúc file
 * @param   *fLineData Con trỏ tới mảng chứa data
 * @param   *fp Con trỏ tới file 
 * @return  char Trả về 0 nếu kết thúc file, 1 nếu chưa kết thúc file
 *******************************************************************************************/
char ReadLine(FILE *fp, char *fLineData){
    // khởi tạo các giá trị
    char fData;
    uint8_t Count=0;

    while((fData = fgetc(fp)) != '\n' && fData != EOF){ //Lặp và gán fData tới khi gặp ký tự \n hoặc EOF
        fLineData[Count] = fData; // gán giá trị lấy được từ file vào mảng chứa data
        Count++;
    } 
    if (fData == EOF) {
        return 0; //EndFile
    }
    return 1; 
}
 /******************************************************************************************
 * @brief   Chuyển đổi từ String sang HEX
 * @details Hàm này thực hiện kiểm tra các phần tử trong mảng với độ dài biết trước
 *          thuộc từ 1 tới 9, từ A tới F sau đó chuyển đổi String sang Hex 
 * @param   *buff  Con trỏ tới mảng cần chuyển đổi
 * @param   length Số lượng phần tử trong mảng cần chuyển đổi
 * @return  void
 *******************************************************************************************/
void ConvertStringToHex(char *buff,uint8_t length){
    // khởi tạo các giá trị
    uint32_t Count = 0; 
    uint32_t Count1 = 0; 
    uint32_t fbuff[50]; 

    for(Count = 0; Count < length; Count++){
        if(buff[Count] >= '0' && buff[Count] <= '9'){ // kiểm tra xem index của mảng có nằm trong khoảng từ 1 tới 9
            fbuff[Count] = buff[Count] - 48; // nếu có thì thực hiện chuyển đổi thành Dec
        }
        else if(buff[Count] >= 'A' && buff[Count] <= 'F'){// kiểm tra xem index của mảng có nằm trong khoảng từ A tới F
            fbuff[Count] = buff[Count] - 65 + 10; // nếu có thì thực hiện chuyển đổi thành Dec
        }
        else{
            return; // nếu không nằm trong khoảng từ 1 tới 9 hoặc A tới F thì thoát vòng lặp
        }
        if(Count%2 != 0){ // kiểm tra xem số lượng phần tử đã chuyển đổi có phải là 2 chưa
            LineHexData[Count1] = ((fbuff[Count-1] << 4) | (fbuff[Count])); //thực hiện chuyển đổi
            Count1++;//index +1
        }
    }
}
 /******************************************************************************************
 * @brief   Chuyển đổi từ HEX sang DEC
 * @details trả về giá trị DEC tương ứng của HEX
 * @param   hexValue giá trị Hex cần chuyển đổi
 * @return  void
 *******************************************************************************************/
uint8_t ConvertHexToDecimal(uint8_t hexValue) {
    return hexValue; // Nếu hexValue là kiểu số, nó đã là DEC
}