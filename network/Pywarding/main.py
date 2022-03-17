#Files
from src import fct_file
from src import fct_loop
from src import fct_signal
from src import fct_display


print("---- Start program ----")
#-------------

#Show parameters
fct_display.show_parameter()

# [SSD] Create directories
fct_file.check_directories()

# [LIDAR] Ask for ethernet network
fct_loop.lidar_2_loop()
fct_loop.lidar_1_loop()

# [STATS] Show final statistics
fct_display.show_stat()

#-------------
print("---- Stop program ----")
