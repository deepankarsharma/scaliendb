import os
import sys
import struct
import signal
import string

def read_data(fmt, data):
	size = struct.calcsize(fmt)
	struct_buf = data[:size]
	data = data[size:]
	ret = [data]
	t = struct.unpack(fmt, struct_buf)
	ret.extend(list(t))
	return tuple(ret)

class StoragePage:
	def __init__(self):
		self.offset = 0
		self.size = 0
		
	def read_data(self, fmt, data):
		size = struct.calcsize(fmt)
		struct_buf = data[:size]
		data = data[size:]
		ret = [data]
		t = struct.unpack(fmt, struct_buf)
		ret.extend(list(t))
		return tuple(ret)
	
	def extract_data(self, length, data):
		return data[length:], data[:length]
		
class StorageHeaderPage(StoragePage):
	def __init__(self):
		self.chunk_id = 0
		self.min_log_segment_id = 0
		self.max_log_segment_id = 0
		self.max_log_command_id = 0
		self.num_keys = 0
		self.use_bloomfilter = False
		self.index_page_offset = 0
		self.index_page_size = 0
		self.bloom_page_offset = 0
		self.bloom_page_size = 0
		self.first_key = ""
		self.last_key = ""
		self.mid_point = ""
	 
	def read(self, file):
		data = file.read(4096)
		if len(data) != 4096:
			return False
		data, size, checksum, version = self.read_data("<III", data)
		# skip text
		text = data[:64]
		data = data[64:]
		data, self.chunk_id, self.min_log_segment_id, self.max_log_segment_id, self.max_log_command_id = \
			self.read_data("<QQQI", data)
		if data[0] == 'T':
			self.use_bloomfilter = True
		data = data[1:]
		data, self.num_keys, self.index_page_offset, self.index_page_size = self.read_data("<QQI", data)
		if self.use_bloomfilter:
			data, self.bloom_page_offset, self.bloom_page_size = self.read_data("<QI", data)
		
		data, length, = self.read_data("<I", data)
		data, self.first_key, = self.extract_data(length, data)

		data, length, = self.read_data("<I", data)
		data, self.last_key, = self.extract_data(length, data)

		data, length, = self.read_data("<I", data)
		data, self.mid_point, = self.extract_data(length, data)

class StorageDataPage(StoragePage):
	def __init__(self, index):
		self.index = index
		self.size = 0
		self.offset = 0
		self.num_keys = 0
		self.keybuffer_length = 0
		self.checksum = 0
		self.key_values = {}

	def read(self, file):
		data = file.read(4096)
		self.size, self.checksum, self.keybuffer_length, self.num_keys = struct.unpack("<IIII", data[:16])
		if self.size > 4096:
			data += file.read(self.size - 4096)
		data = data[16:]
		# print("-- Page index %d, Num keys: %d" % (self.index, self.num_keys))
		i = 0
		prev_key = ""
		while i < self.num_keys:
			type, keylen = struct.unpack("<cH", data[:3])
			data = data[3:]
			key = data[:keylen]
			if key <= prev_key:
				print("Error")
			prev_key = key
			data = data[keylen:]
			# TODO: values
			# if type == 's':
			# 	vallen, = struct.unpack("<I", data[:4])
			# 	data = data[4:]
			# 	value = data[:vallen]
			# 	data = data[vallen:]
			# 	if self.key_values.has_key(key):
			# 		print("Error")
			# 		print(file.tell())
			# 	self.key_values[key] = value
			# else:
			# 	print("Delete key: " + key)
			# 	if self.key_values.has_key(key):
			# 		self.key_values.pop(key)
			# print(key, value)
			# print(key)
			i += 1
		return i

class StorageIndexPage(StoragePage):
	def __init__(self):
		self.size = 0
		self.offset = 0
		self.checksum = 0
		self.num_keys = 0
	
	def read(self, file):
		data = file.read(4096)
		self.size, self.checksum, self.num_keys = struct.unpack("<III", data[:12])
		if self.size > 4096:
			data += file.read(self.size - 4096)
		data = data[12:]
		i = 0
		print("Index num_keys: %d, data length: %d" % (self.num_keys, len(data)))
		while i < self.num_keys:
			# key_offset, keylen = struct.unpack("<QH", data[:10])
			# data = data[10:]
			data, key_offset, keylen = self.read_data("<QH", data)
			key = data[:keylen]
			data = data[keylen:]
			i += 1
			# print("Index keylen: %d" % (keylen))
			# print("Index key: %s" % (key))

class StorageBloomPage(StoragePage):	
	def __init__(self):
		self.size = 0
		self.offset = 0
		self.checksum = 0

	def read(self, file):
		data = file.read(4096)
		self.size, self.checksum = struct.unpack("<II", data[:8])
		if self.size > 4096:
			data += file.read(self.size - 4096)
		data = data[8:]
		print("-- Bloom page size: %d" % (self.size))

def check_chunk_file(filename):
	f = open(filename, "rb")
	if os.path.getsize(filename) == 0:
		print("Empty chunk file")
		return
	header_page = StorageHeaderPage()
	header_page.read(f)
	print("num_keys = " + str(header_page.num_keys))
	print(str(header_page.__dict__))
	keys = 0
	index = 0
	while keys < header_page.num_keys:
		data_page = StorageDataPage(index)
		keys += data_page.read(f)
		index += 1
		#print("Keys: " + str(keys))
	index_page = StorageIndexPage()
	index_page.read(f)
	if header_page.use_bloomfilter:
		bloom_page = StorageBloomPage()
		bloom_page.read(f)

if __name__ == "__main__":
	signal.signal(signal.SIGPIPE, signal.SIG_DFL)
	check_chunk_file(sys.argv[1])
