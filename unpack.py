import struct
import os

def pack_files_back(input_dir, output_file):
    
    file_list = []

    
    for root, dirs, files in os.walk(input_dir):
        for file in files:
            
            file_path = os.path.join(root, file)
            
            rel_path = os.path.relpath(file_path, input_dir)[:48].ljust(48, '\x00')
           
            file_size = os.path.getsize(file_path)
           
            file_list.append((file_path, rel_path, file_size))

    
    with open(output_file, 'wb') as file:
        
        header = b'DATA$TOP' + b'\x00' * 48
        
        file.write(header)

        
        num_files = len(file_list)
        num_files += 1
        
        file.write(struct.pack('<H', num_files))
        num_files -= 1
        
        file.write(b'\x00' * 6)

        index_offset = 0

        
        for i, (file_path, rel_path, size) in enumerate(file_list):
            
            file.write(rel_path.encode('ascii'))
            
            file.write(struct.pack('<I', index_offset))
            
            file.write(struct.pack('<I', index_offset))
            
            file.write(struct.pack('<I', size))
            
            file.write(b'\x00' * 4)
            index_offset += size

        
        for file_path, _, _ in file_list:
           
            with open(file_path, 'rb') as input_file:
                
                data = input_file.read()
                
                file.write(data)


input_dir = ""
output_file = ""


pack_files_back(input_dir, output_file)
