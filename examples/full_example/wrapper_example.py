import shmaro
from multiprocessing import Process, Queue
from Queue import Empty
import threading
import time
import sys
import Tkinter
import matplotlib.pyplot as plt



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
	rpm_list = []
	engineload_list = []
	airflow_list = []
	throttle_list = []
	coolanttmp_list = []
	speed_list = []
	intaketmp_list = []
	fuel_list = []


	counter = 0

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
				#for x in range(0, len(tr)):
				#	print("RPM: " + str(tr[x].get_rpm()) + " LD: " + str(tr[x].get_load()) + " AF: " + str(tr[x].get_throttle()))
				while counter < len(tr):
					rpm_list.append(tr[counter].get_rpm())
					engineload_list.append(tr[counter].get_load())
					airflow_list.append(tr[counter].get_airflow())
					throttle_list.append(tr[counter].get_throttle())
					coolanttmp_list.append(tr[counter].get_coolant_tmp())
					speed_list.append(tr[counter].get_speed())
					intaketmp_list.append(tr[counter].get_intake_tmp())
					fuel_list.append(tr[counter].get_fuel_status())
					#print("RPM: {}".format(rpmlist[counter]))
					counter = counter + 1
				print("")
			else:
				time.sleep(1)
		except Empty:
			print "ERROR"
		
	
	p1.terminate()


	top = Tkinter.Tk()
	def display_rpm_Data():
		print 'rpmData'
		#print len(rpm_list)
		plt.plot(rpm_list) #uses a list
		plt.ylabel('RPM')
		plt.show()
	def display_engineload_Data():
		print 'loadData'
		#print len(engineload_list)
		plt.plot(engineload_list) #uses a list
		plt.ylabel('Engine Load')
		plt.show()
	def display_airflow_Data():
		print 'airflowData'
		#print len(airflow_list)
		plt.plot(airflow_list) #uses a list
		plt.ylabel('AirFlow')
		plt.show()
	def display_throttle_Data():
		print 'throttleData'
		#print len(throttle_list)
		plt.plot(throttle_list) #uses a list
		plt.ylabel('Throttle')
		plt.show()
	def display_coolanttmp_Data():
		print 'coolanttmpData'
		#print len(coolanttmp_list)
		plt.plot(coolanttmp_list) #uses a list
		plt.ylabel('Coolant Temperature')
		plt.show()
	def display_speed_Data():
		print 'speedData'
		#print len(speed_list)
		plt.plot(speed_list) #uses a list
		plt.ylabel('Speed')
		plt.show()
	def display_intaketmp_Data():
		print 'intaketmpData'
		#print len(intaketmp_list)
		plt.plot(intaketmp_list) #uses a list
		plt.ylabel('Intake Temperature')
		plt.show()
	def display_fuel_Data():
		print 'fuelData'
		#print len(fuel_list)
		plt.plot(fuel_list) #uses a list
		plt.ylabel('Fuel')
		plt.show()

	rpm_button = Tkinter.Button(top, text = "RPM", command = display_rpm_Data)
	load_button = Tkinter.Button(top, text = "Engine Load", command = display_engineload_Data)
	airflow_button = Tkinter.Button(top, text = "Air Flow", command = display_airflow_Data)
	throttle_button = Tkinter.Button(top, text = "Throttle", command = display_throttle_Data)
	coolant_button = Tkinter.Button(top, text = "Coolant Temp", command = display_coolanttmp_Data)
	speed_button = Tkinter.Button(top, text = "Speed", command = display_speed_Data)
	intake_button = Tkinter.Button(top, text = "Intake Temp", command = display_intaketmp_Data)
	fuel_button = Tkinter.Button(top, text = "Fuel Status", command = display_fuel_Data)
	rpm_button.pack()
	load_button.pack()
	airflow_button.pack()
	throttle_button.pack()
	coolant_button.pack()
	speed_button.pack()
	intake_button.pack()
	fuel_button.pack()
	top.mainloop()


	


	

	

	print "Successfully closed Program"		