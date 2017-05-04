import shmaro
from multiprocessing import Process, Queue
from Queue import Empty
import threading
import time
import sys


def Shmaro_Communicator(shared, done):
	while(done == False):
		shared.put(0)
		shared.put(0)
		#time.sleep(1)
		shmaro.start_server()
		obj = shmaro.get_list()
		obj_list = []
		for x in range(0, len(obj)):
			if(obj[x].get_flag() != 2):
				obj_list.append(obj[x])
			if(obj[x].get_flag() == 1):
				done = True
		shared.put(obj_list)
		#time.sleep(1)
	shared.put(True)
	exit(0)


if __name__=='__main__':
	shared = Queue()
	done = False
	p1 = Process(target=Shmaro_Communicator, args=(shared,done))
	p1.start()
	num = 0
	while(True):
		try:
			tr = any
			tr = shared.get()
			if(tr == True):
				break
			if(tr != 0):
				for x in range(0, len(tr)):
					print("RPM: " + str(tr[x].get_rpm()) + " LD: " + str(tr[x].get_load()))
				print("")
		except Empty:
			pass
		except:
			exit(0)
			
		#lock.release()
	p1.terminate()
	print "Successfully closed Program"		