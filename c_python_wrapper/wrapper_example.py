import shmaro
from multiprocessing import Process, Queue
from Queue import Empty
import threading
import time
import sys


def Shmaro_Communicator(shared, done):
	while(done == False):
		# shmaro start_server functions as a listener.
		# it will block until it recieves a packet. 
		# Call it everytime you need to have the socket
		# open
		shmaro.start_server()
		# get list retrieves a list of engine objects
		obj = shmaro.get_list()
		obj_list = []
		# The list is not native and must be converted so python can put them in shared
		# memory
		for x in range(0, len(obj)):
			if(obj[x].get_flag() != 2):
				obj_list.append(obj[x])
			if(obj[x].get_flag() == 1):
				done = True
		# Put list into shared memory to be retrieved by main process
			shared.put(obj_list)
	shared.put(True)
	exit(0)


if __name__=='__main__':
	# Queue to pass between socket process and main process
	shared = Queue()
	done = False
	p1 = Process(target=Shmaro_Communicator, args=(shared,done))
	p1.start()
	num = 0
	while(done is False):
		try:
			tr = any
			tr = shared.get()
			if(tr == True):
				done = True
				break
			if(tr != 0):
				for x in range(0, len(tr)):
					print("RPM: " + str(tr[x].get_rpm()) + " LD: " + str(tr[x].get_load()))
				print("")
			else:
				time.sleep(1)
		except Empty:
			print "ERROR"
		#except:
		#	print "ERrror"
			
		#lock.release()
	p1.terminate()
	print "Successfully closed Program"		