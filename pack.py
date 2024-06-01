import struct
import os

def unpack_all_files(pack_file, output_dir):
    zero_size_count = 0
    extracted_count = 0 

    try:
        with open(pack_file, 'rb') as file:
            file.seek(56)
            num_files_bytes = file.read(2) 
            num_files_bytes = num_files_bytes[::-1]
            num_files = struct.unpack('>H', num_files_bytes)[0]
            num_files = num_files -1               
            print("num_files:", num_files)

            for i in range(num_files):
                file.seek(64 + i * 64)  
                file_name_bytes = file.read(48)
                file_name = file_name_bytes.decode('utf-8', 'ignore').replace('\x00', '')   
                
                file.seek(112+ i * 64)   
                offset_bytes = file.read(4)
                offset_bytes = offset_bytes[::-1]
                offset = struct.unpack('>I', offset_bytes)[0]   

                file.seek(120+ i * 64)  
                size_bytes = file.read(4)
                size_bytes = size_bytes[::-1]
                size = struct.unpack('>I', size_bytes)[0]       

                if size == 0:
                    zero_size_count += 1

                start_offset = (num_files + 1) * 64 + offset     
                end_offset = start_offset + size         

                file.seek(start_offset)
                data = file.read(size)

                output_path = os.path.join(output_dir, file_name)
                print("Output Path:", output_path)
                
                # 创建输出目录中的子目录
                output_subdir = os.path.dirname(output_path)
                if not os.path.exists(output_subdir):
                    os.makedirs(output_subdir)
                
                with open(output_path, 'wb') as output_file:
                    output_file.write(data)

                extracted_count += 1  
                print("Extracted file:", file_name)
                print("End offset of the file:", hex(end_offset))
                print("num:", i+1)

    except Exception as e:
        print("An error occurred during extraction:", str(e))

    print("Number of files with size 0:", zero_size_count)
    print("Successfully extracted", extracted_count, "files")  

pack_file = ""
output_dir = ""
unpack_all_files(pack_file, output_dir)
