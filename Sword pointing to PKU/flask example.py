"""
New Version Feature:
Remove the git pull action

"""
from flask import Flask , render_template , request , url_for , send_from_directory , redirect , jsonify
from celery import Celery
import time
import datetime
import os
import csv
import sqlite3
from flask_cors import CORS



app = Flask(__name__)
CORS(app)

app.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'
app.config['CELERY_RESULT_BACKEND'] = 'redis://localhost:6379/0'
celery = Celery(app.name, broker=app.config['CELERY_BROKER_URL'])
celery.conf.update(app.config)
dbpath = "/root/yan.zhang/CodeSearch/search.db"



def search_project(access_token , group_name , project_name , keyword , workpath, csv_writer):
	os.chdir(workpath + '/src')
	a = os.system('cd ' + project_name )
	if a != 0 :
		cmd1='git clone http://oauth2:'+access_token +'@'+'git.sami.int.thomsonreuters.com/'+ group_name +'/' + project_name + '.git' 
		if os.system(cmd1) != 0:
			return 'Wrong project_name or group_name'
	os.chdir(workpath + '/src/' + project_name)
	#os.system('git pull')
	remotebranch_list = os.popen('git branch -a').read().split('\n')
	for remotebranch in remotebranch_list:
		if ('origin/' not in remotebranch) or ('HEAD' in remotebranch):
			continue 
		branch = remotebranch.split('origin/')[1]
		if os.system('git checkout ' + branch) != 0:
			os.system('git checkout -b ' + branch  + ' origin/' + branch)
		result_list = os.popen('grep -rn "'+keyword+'" *').read().split('\n')[:-1]
		if result_list == []:
			insert_row = []
			insert_row.append(project_name)
			insert_row.append(branch)
			insert_row.append('NULL')
			csv_writer.writerow(insert_row)
		for result in result_list:
			insert_row = []
			insert_row.append(project_name)
			insert_row.append(branch)
			insert_row.append(result.split(':',2)[-1])
			insert_row.append(result.split(':')[0])
			csv_writer.writerow(insert_row)

def search_mte(access_token , project, keyword , workpath , csv_writer):
	project_name = project.split('/')[-1].split('.git')[0]
	group_name = project.split('/')[-2]
	search_project(access_token , group_name , project_name , keyword , workpath, csv_writer)

"""
def search_group(access_token , group_name , keyword , workpath , csvfile_name):
	outputfile = open(csvfile_name, 'wb')
	csv_writer = csv.writer(outputfile)
	templist = ['Venue','Branch','Value','Path']
	csv_writer.writerow(templist)

	filelist = open("venue_list.txt","r")
	project_list=filelist.read().split('\n')
	filelist.close()
	#project_list = ['krx_lookup']
	for project_name in project_list:
		search_project(access_token , group_name ,project_name, keyword , workpath , csv_writer)
	outputfile.close()
	os.system('mv '+csvfile_name+' '+workpath+'/result')
"""

def add_task_to_table(taskid,keyword,filename,state,username):
	conn = sqlite3.connect(dbpath)
	cursor = conn.cursor()
	values = "('"+taskid+"','"+keyword+"','"+filename+"','"+state+"','"+username+"',0,1)"
	sql = "insert into TASK(taskid,keyword,filename,state,username,curr,total) values "+values
	cursor.execute(sql)
	cursor.close()
	conn.commit()
	conn.close()

def update_task_status_to_table(taskid,state,curr,total):
	conn = sqlite3.connect(dbpath)
	cursor = conn.cursor()
	sql = " UPDATE TASK SET state = '"+state+"', curr = "+str(curr)+", total ="+str(total)+" WHERE taskid ='"+taskid+"'"
	cursor.execute(sql)
	cursor.close()
	conn.commit()
	conn.close()

def get_user_task_from_table(username):
	conn = sqlite3.connect(dbpath)
	cursor = conn.cursor()
	sql = "SELECT * from TASK WHERE username = '"+username+"'"
	cursor.execute(sql)
	values = cursor.fetchall()
	response = []
	for value in values:
		response.append({'taskid':value[0],'keyword':value[1],'filename':value[2],'state':value[3],'curr':value[5],'total':value[6]})
	cursor.close()
	conn.commit()
	conn.close()
	return response




@celery.task(bind=True)
def search_task(self , group_name , keyword , csvfile_name):
	workpath = '/root/yan.zhang/CodeSearch'
	access_token = 'mVDPyxxgbYm2auxf8Az_'
	os.chdir(workpath)

	outputfile = open(csvfile_name, 'wb')
	csv_writer = csv.writer(outputfile)
	templist = ['Venue','Branch','Value','Path']
	csv_writer.writerow(templist)

	if group_name == 'thunderdome_venues':
		filelist = open("venue_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		#project_list = ['krx_lookup','SHFE-FTE']
		i=0
		total = len(project_list)
		for project_name in project_list:
		#for i in range(10):
			search_project(access_token , group_name ,project_name, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total , 'file' :csvfile_name})
	elif group_name == 'thunderdome_puppet_components' :
		filelist = open("puppet_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		i=0
		total = len(project_list)
		for project_name in project_list:
			search_project(access_token , group_name ,project_name, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total ,'file' :csvfile_name })
	else:
		filelist = open("mte_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		i = 0
		total = len(project_list)
		for project in project_list:
			search_mte(access_token , project, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total,'file' :csvfile_name})

	os.chdir(workpath)
	outputfile.close()
	os.system('mv '+csvfile_name+' '+workpath+'/result')
	return "OK"

@app.route("/")
def index():
	return render_template("search.html")



@app.route("/status/<task_id>")
def taskstatus(task_id):
    task = search_task.AsyncResult(task_id)
    if task.state == 'PENDING':
        state = task.state
        current = 0
        total  = 1
        status= "Pending..."
        filecsv ="Pending..."
    elif task.state == 'PROGRESS':
        state = task.state
        current = task.info.get('current')
        total = task.info.get('total')
        filecsv = task.info.get('file')
        status = task.state
    elif task.state == 'SUCCESS':
    	return redirect('http://167.76.120.70:8000/re')
    else:
    	return task.state
    return render_template("taskstatus.html",sta = state , cur=current,tot=total,stu = status ,fi = filecsv )

@app.route("/search" , methods = ['POST'])
def search():
  keyword = request.form.get("keyword")
  group = request.form.get("group")

  curr_time =time.strftime('%Y%m%d%H%M%S',time.gmtime(time.time()))
  tempkeyword = filter(str.isalnum, keyword.encode('unicode-escape').decode('string_escape'))
  newkeyword = ''.join(list(tempkeyword))
  csvfile_name = curr_time + newkeyword +'.csv'

  if group == 'venue':
  	group_name = 'thunderdome_venues'
  elif group == 'puppet':
  	group_name = 'thunderdome_puppet_components'
  else:
  	group_name = 'mte'
  task = search_task.apply_async(args=[group_name , keyword ,csvfile_name])
  return render_template("createtask.html" , taskid = task.id , kd = keyword , filename = csvfile_name)

@app.route("/re")
def dirlist():
  flist=os.listdir("/root/yan.zhang/CodeSearch/result")
  return render_template('result.html',fl=flist)

@app.route("/result/<name>")
def filex(name):
    return send_from_directory(directory='/root/yan.zhang/CodeSearch/result',filename = name,as_attachment=True)







@celery.task(bind=True)
def test_search_task(self , group_name , keyword , csvfile_name , username):
	workpath = '/root/yan.zhang/CodeSearch'
	access_token = 'mVDPyxxgbYm2auxf8Az_'
	cur_task_id = self.request.id
	os.chdir(workpath)
	outputfile = open(csvfile_name, 'wb')
	csv_writer = csv.writer(outputfile)
	templist = ['Venue','Branch','Value','Path']
	csv_writer.writerow(templist)
	if group_name == 'thunderdome_venues':
		filelist = open("venue_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		i=0
		total = len(project_list)
		for project_name in project_list:
			search_project(access_token , group_name ,project_name, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total , 'file' :csvfile_name})
			update_task_status_to_table(cur_task_id,"PROGRESS",i,total)
	elif group_name == 'thunderdome_puppet_components' :
		filelist = open("puppet_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		i=0
		total = len(project_list)
		for project_name in project_list:
			search_project(access_token , group_name ,project_name, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total ,'file' :csvfile_name })
			update_task_status_to_table(cur_task_id,"PROGRESS",i,total)
	else:
		filelist = open("mte_list.txt","r")
		project_list=filelist.read().split('\n')
		filelist.close()
		i = 0
		total = len(project_list)
		for project in project_list:
			search_mte(access_token , project, keyword , workpath , csv_writer)
			i+=1
			self.update_state(state='PROGRESS',meta={'current': i, 'total': total,'file' :csvfile_name})
			update_task_status_to_table(cur_task_id,"PROGRESS",i,total)
	os.chdir(workpath)
	outputfile.close()
	user_file_path  = workpath + '/result/'+username
	findir = os.path.exists(user_file_path)
	if not findir:
		os.makedirs(user_file_path) 
	os.system('mv '+csvfile_name+' '+user_file_path)
	update_task_status_to_table(cur_task_id,"SUCCESS",1,1)
	return "OK"

@app.route("/test")
def testindex():
	return render_template("testsearch.html")


@app.route("/test/status/<task_id>")
def test_task_status(task_id):
    task = search_task.AsyncResult(task_id)
    if task.state == 'PROGRESS':
        response = {
        'state' : task.state,
        'current' : task.info.get('current'),
        'total' : task.info.get('total'),
        'filecsv' : task.info.get('file'),
        'status' : task.state
        }
    elif task.state == 'SUCCESS':
    	response ={
    	'state' : task.state
    	}
    else:
    	response ={
    	'state' : task.state
    	}
    return jsonify(response)


@app.route("/test/search" , methods = ['POST','GET'])
def test_search():
  keyword = request.json.get("keyword")
  group = request.json.get("group")
  username = request.json.get("username")

  curr_time = time.strftime('%Y%m%d%H%M%S',time.gmtime(time.time()))

  tempkeyword = filter(str.isalnum, keyword.encode('unicode-escape').decode('string_escape'))
  newkeyword = ''.join(list(tempkeyword))
  csvfile_name = curr_time + newkeyword +'.csv'

  if group == 'venue':
  	group_name = 'thunderdome_venues'
  elif group == 'puppet':
  	group_name = 'thunderdome_puppet_components'
  else:
  	group_name = 'mte'
  task = test_search_task.apply_async(args=[group_name , keyword ,csvfile_name , username])
  task_id = task.id
  add_task_to_table(task_id,keyword,csvfile_name,"PENDING",username)
  response = {
	'taskid' : task_id,
	'keyword' : keyword,
	'filename' : csvfile_name,
	'username' : username
  }
  return jsonify(response)

@app.route("/test/re")
def test_dirlist():
  flist=os.listdir("/root/yan.zhang/CodeSearch/result")
  response = {}
  serverpath = "http://167.76.120.70:8000/re/"
  for fl in flist:
  	response.update({fl: serverpath+fl})
  return jsonify(response)

@app.route("/test/result/<username>")
def test_get_user_file_list(username):
	Path = "/root/yan.zhang/CodeSearch"
	path = Path+"/result/"+username
	folder = os.path.exists(path)
	if not folder:
		return "None"
	else:
		flist = flist=os.listdir(path)
		response = {}
		serverpath = "http://167.76.120.70:8000/test/result/"+ username +"/" 
		for fl in flist:
			response.update({fl: serverpath+fl})
	return jsonify(response)

@app.route("/test/result/<username>/<name>")
def test_get_user_file(username , name):
    return send_from_directory(directory='/root/yan.zhang/CodeSearch/result/'+username,filename = name,as_attachment=True)

@app.route("/test/getuseralltask/<username>")
def get_user_all_task(username):
	return jsonify(get_user_task_from_table(username))


if __name__ == '__main__' :
  app.run( host = '0.0.0.0' , port = 8000 )
