import math
import struct
import numpy as np

from HSD.utils.type_conversion import TypeConversion

from .DataClass import DataClass

class DataReader(object):
    def __init__(self, controller, comp_name, samples_per_ts, dimensions, sample_size, data_format):
        self.controller = controller
        self.comp_name = comp_name
        self.samples_per_ts = samples_per_ts
        self.dimensions = dimensions
        self.sample_size = sample_size
        if samples_per_ts != 0:
            self.data_size = self.sample_size * self.samples_per_ts * self.dimensions
            self.time_size = TypeConversion.check_type_length("double")
            self.time_format = "d"
        else:
            self.data_size = self.sample_size * self.dimensions
            self.time_size = 0
            self.time_format = ""
        self.data_format = data_format
        self.next_is_timestamp = False

        self.data_buffer = []
        self.data_dict = dict()

        self.data_counter = 0
        self.rem_dim_bytes = ""

    def get_largest_mult_of_n_lower_than_m(self, n, m):
        return int((m // n) * n)  # Largest multiple of n lower than m

    def calculate_data_to_extract(self, data):
        # if data lenght is multiple of dimensions
        int_rem_data_bytes = len(data)
        data_to_extract = int_rem_data_bytes / self.sample_size
        # else, it is needed to select the largest multiple of dimensions lower than the length of data_to_extract
        if data_to_extract % self.dimensions != 0:
            data_to_extract = self.get_largest_mult_of_n_lower_than_m(self.dimensions, data_to_extract)
            int_rem_data_bytes = self.sample_size * data_to_extract
        return [int_rem_data_bytes, data_to_extract]
        
    def update_data_counter_and_time_flag(self):
        if self.data_counter == self.samples_per_ts:
            self.data_counter = 0
            self.next_is_timestamp = True
        else:
            self.next_is_timestamp = False

    def fill_data_buffer(self, data):
        if len(data) <= self.data_size: # data length is less than the full packet data size (sample_size * samples_size_per_ts * size)
            # calculate the right amount of data to extract
            [int_rem_data_bytes, data_to_extract] = self.calculate_data_to_extract(data)
            # update the data_counter
            self.data_counter = int(data_to_extract) 
            # copy data into buffer
            self.data_buffer = np.frombuffer(data[:int_rem_data_bytes], dtype=self.data_format, count=int(data_to_extract))
            # take the remaining bytes, if any (e.g., if data finish with a non complete sample (x,y,) z missing)
            self.rem_dim_bytes = data[int_rem_data_bytes:]
            # update data_counter (remain the same) and timestamp flag (to False)
            self.update_data_counter_and_time_flag()
        else:
            # extract the number of complete packet (data+timestamp)
            n_cplt_packet = math.floor(len(data) / (self.data_size + self.time_size))
            # create the format string to unpack the entire buffer (if samples_per_ts is not 0 add also timestamp data position)
            if self.samples_per_ts != 0:
                format = "=" + str(n_cplt_packet * (str(str(self.samples_per_ts * self.dimensions) + self.data_format) + (str(self.time_format))))
            else:
                format = "=" + str(n_cplt_packet * (str(str(self.dimensions) + self.data_format)))
            # unpack data from the byte raw data buffer
            data_t = data[: n_cplt_packet * (self.data_size + self.time_size)]
            self.data_buffer = list(struct.unpack(format, data_t))
            # remove timestamps extracted
            # DEBUG
            # print("residual timestamps -->", self.data_buffer[(self.samples_per_ts * self.dimensions) :: (self.samples_per_ts * self.dimensions) + 1])
            if self.samples_per_ts != 0:
                del self.data_buffer[(self.samples_per_ts * self.dimensions) :: (self.samples_per_ts * self.dimensions) + 1]
            else:
                del self.data_buffer[self.dimensions :: self.dimensions + 1]
            # take the remaining bytes
            self.rem_data_bytes = data[n_cplt_packet * (self.data_size + self.time_size):]
            # calculate the right amount of data to extract
            [int_rem_data_bytes, data_to_extract] = self.calculate_data_to_extract(self.rem_data_bytes)
            # update the data_counter
            self.data_counter = int(data_to_extract)
            # add remaining extracted data into the data_buffer
            self.data_buffer.extend(np.frombuffer(data[n_cplt_packet * (self.data_size + self.time_size) : n_cplt_packet * (self.data_size + self.time_size) + int_rem_data_bytes], dtype=self.data_format, count=self.data_counter))
            # take the remaining bytes, if any (e.g., if data finish with a non complete sample (x,y,) z missing)
            self.rem_dim_bytes = data[ n_cplt_packet * (self.data_size + self.time_size) + int_rem_data_bytes :]
            # update data_counter and timestamp flag
            self.update_data_counter_and_time_flag()

    def feed_data(self, data):
        if self.comp_name == self.comp_name:
            if len(self.rem_dim_bytes) != 0:
                data.data = self.rem_dim_bytes + data.data
            if self.data_counter == 0:
                if not self.next_is_timestamp:
                    # received data starts with data
                    self.fill_data_buffer(data.data)
                else:
                    # received data starts with timestamp
                    data.data = data.data[self.time_size :]  # removing the timestamp
                    self.fill_data_buffer(data.data)
            else:
                #calculate the difference in order to determine missing data to obtain a complete packet (samples_per_ts * dimensions)
                diff = (self.samples_per_ts * self.dimensions) - self.data_counter  # data_counter counts data. In case of multiple dim data one dimension is one data
                if (len(data.data) < diff * self.sample_size):  # data is raw bytes, to get bytes from diff (samples) it is necessary to multiply it for sample size
                    # calculate the right amount of data to extract
                    [int_rem_data_bytes, data_to_extract] = self.calculate_data_to_extract(data.data)
                    # copy data into buffer
                    self.data_buffer = np.frombuffer(data.data[:int_rem_data_bytes], dtype=self.data_format, count=int(int_rem_data_bytes / self.sample_size))
                    # update the data_counter
                    self.data_counter += int(data_to_extract)
                    # take the remaining bytes, if any (e.g., if data finish with a non complete sample (x,y,) z missing)
                    self.rem_dim_bytes = data.data[int_rem_data_bytes:]
                else:
                    # copy data into buffer
                    self.data_buffer = np.frombuffer(data.data[: diff * self.sample_size], dtype=self.data_format, count=diff)
                    # reinit data_counter
                    self.data_counter = 0
                    if len(self.data_buffer) == (len(data.data) / self.sample_size):
                        #next received packet will start with a timestamp
                        self.next_is_timestamp = True
                    else:
                        # removing the timestamp (excluding the first)
                        data.data = data.data[(len(self.data_buffer) * self.sample_size)+ self.time_size :]
                        # fill data buffer
                        self.fill_data_buffer(data.data)
           
            if len(self.data_buffer) > 0:
                #split data in dictionary for each dimension
                for i in range(self.dimensions):
                    self.data_dict[i] = np.array(self.data_buffer[i::self.dimensions])
                #Add data to a plot
                self.controller.add_data_to_a_plot(DataClass(self.comp_name, self.data_dict))

