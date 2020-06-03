from flask import Flask , render_template , request , url_for , send_from_directory , redirect , jsonify
from celery import Celery
import time
import datetime
import os
import csv
import sqlite3
from flask_cors import CORS

#Initialize flask server
app = Flask(__name__)
CORS(app)

#Config Celery
app.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'
app.config['CELERY_RESULT_BACKEND'] = 'redis://localhost:6379/0'
celery = Celery(app.name, broker=app.config['CELERY_BROKER_URL'])
celery.conf.update(app.config)

#Define the database path and workpath
dbpath = "../search.db"
workpath = "../CodeSearch"

#Config git access token
access_token = ''


#Database Operation Code 
#Operations about table TASK
def add_task_to_table(taskid,keyword,filename,state,username):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		values = "('"+taskid+"','"+keyword+"','"+filename+"','"+state+"','"+username+"',0,1)"
		sql = "insert into TASK(taskid,keyword,filename,state,username,curr,total) values "+values
		cursor.execute(sql)
		cursor.close()
		conn.commit()
		conn.close()
		return "OK"
	except DBERROR:
		return "ERROR in DB"

def update_task_status_to_table(taskid,state,curr,total):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		sql = " UPDATE TASK SET state = '"+state+"', curr = "+str(curr)+", total ="+str(total)+" WHERE taskid ='"+taskid+"'"
		cursor.execute(sql)
		cursor.close()
		conn.commit()
		conn.close()
		return "OK"
	except DBERROR:
		return "ERROR in DB"

def get_user_task_from_table(username):
	try:
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
	except DBERROR:
		return "ERROR in DB"


#Operations about table CONFIG
def add_user_config_to_table(project , group , branch , squad , region , username):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		values = "('"+group+"','"+squad+"','"+region+"','"+username+"','"+project+"','"+branch+"')"
		sql = "replace into CONFIG(group_name,squad,region,username,project,branch) values "+values
		cursor.execute(sql)
		values = cursor.fetchall()
		cursor.close()
		conn.commit()
		conn.close()
		return "OK"
	except DBERROR:
		return "ERROR in DB"


def delete_user_config_from_table(recordid):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		sql = "DELETE from CONFIG WHERE recordid = "+ str(recordid)
		cursor.execute(sql)
		cursor.close()
		conn.commit()
		conn.close()
		return "OK"
	except DBERROR:
		return "ERROR in DB"


#next two methods have the same function
def get_user_config_from_table(username):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		sql = "SELECT * from CONFIG WHERE username = '"+username+"'"
		cursor.execute(sql)
		values = cursor.fetchall()
		response = []
		for value in values:
			response.append({'group':value[0],'squad':value[1],'region':value[2],'username':value[3], 'project':value[4],'branch':value[5],'recordid':value[6]})
		cursor.close()
		conn.commit()
		conn.close()
		return response
	except DBERROR:
		return "ERROR in DB"

def get_projects_info_from_table(filed , filedvalue):
	try:
		conn = sqlite3.connect(dbpath)
		cursor = conn.cursor()
		sql = "SELECT * from CONFIG WHERE " + filed + "= '"+filedvalue+"'"
		cursor.execute(sql)
		values = cursor.fetchall()
		response = []
		for value in values:
			response.append({'group':value[0],'squad':value[1],'region':value[2],'username':value[3], 'project':value[4],'branch':value[5],'recordid':value[6]})
		cursor.close()
		conn.commit()
		conn.close()
		return response
	except DBERROR:
		return "ERROR in DB"



#Celery Audit task process code includding audit_task  list_audit project_audit
#Audit a specific project
#This method will be called by list_audit
def project_audit(group_name, project_name , branch_name , keyword , csv_writer ):
	os.chdir(workpath + '/src')
	a = os.system('cd ' + project_name )
	if a != 0 :
		cmd1='git clone http://oauth2:'+access_token +'@'+'git.sami.int.thomsonreuters.com/'+ group_name +'/' + project_name + '.git' 
		if os.system(cmd1) != 0:
			return 'Wrong project_name or group_name'
	os.chdir(workpath + '/src/' + project_name)
	os.system('git pull')
	remotebranch_list = os.popen('git branch -a').read().split('\n')
	for remotebranch in remotebranch_list:
		if ('origin/' not in remotebranch) or ('HEAD' in remotebranch):
			continue 
		if(branch_name == 'ALL') or (branch_name in remotebranch):
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

#Audit Target Project list
#grouptype comes from user's parameter 'group', it means we should audit puppet or TD 
#projects_info is a list get from get_projects_info_from_table.The conetent of the list is projects we need to audit
#this method calls project_audit and will be called by audit_task
def list_audit( grouptype , projects_info , keyword , csv_writer , cur_task_id):
	total = len(projects_info)
	if 'puppet' in grouptype:
		for eachproject in projects_info:
			if 'puppet' not in str(eachproject.get('group')):
				projects_info.remove(eachproject)
	else:
		for eachproject in projects_info:
			if 'puppet' in str(eachproject.get('group')):
				projects_info.remove(eachproject)
	i=0
	for eachproject in projects_info:
		project_audit(eachproject.get('group'),eachproject.get('project'),eachproject.get('branch'),keyword,csv_writer)
		i+=1
		update_task_status_to_table(cur_task_id,"PROGRESS",i,total)

#Celery task process code
#This method will call list_audit and be called by api_audit
@celery.task(bind=True)
def audit_task(self , keyword , csvfile_name , grouptype , username):
	try:
		cur_task_id = self.request.id
		os.chdir(workpath)
		outputfile = open(csvfile_name, 'wb')
		csv_writer = csv.writer(outputfile)
		templist = ['Venue','Branch','Value','Path']
		csv_writer.writerow(templist)
		projects_info = get_projects_info_from_table("username" , username)
		list_audit(grouptype,projects_info,keyword,csv_writer,cur_task_id)
		os.chdir(workpath)
		outputfile.close()
		user_file_path  = workpath + '/result/'+username
		findir = os.path.exists(user_file_path)
		if not findir:
			os.makedirs(user_file_path) 
		os.system('mv '+csvfile_name+' '+user_file_path)
		return update_task_status_to_table(cur_task_id,"SUCCESS",1,1)
	except:
		return update_task_status_to_table(cur_task_id,"Failed",0,1)




#Flask API:
@app.route("/audit" , methods = ['POST','GET'])
def api_audit():
	keyword = request.json.get("keyword")
	grouptype = request.json.get("group")
	username = request.json.get("username")

	curr_time = time.strftime('%Y%m%d%H%M%S',time.gmtime(time.time()))
	tempkeyword = filter(str.isalnum, keyword.encode('unicode-escape').decode('string_escape'))
	newkeyword = ''.join(list(tempkeyword))
	csvfile_name = curr_time + newkeyword +'.csv'
	task = audit_task.apply_async(args=[keyword , csvfile_name , grouptype , username])
	task_id = task.id
	add_task_to_table(task_id,keyword,csvfile_name,"PENDING",username)
	response = {
	'taskid' : task_id,
	'keyword' : keyword,
	'filename' : csvfile_name,
	'username' : username
	}
	return jsonify(response)

@app.route("/adduserconfig" , methods = ['POST','GET'])
def add_user_config():
	project = request.json.get('project')
	group = request.json.get('group')
	branch = request.json.get('branch')
	squad = request.json.get('squad')
	region = request.json.get('region')
	username = request.json.get('username')
	return add_user_config_to_table(project , group , branch , squad , region , username)

@app.route("/getuserconfig/<username>")
def get_user_config(username):
	return jsonify(get_user_config_from_table(username))


@app.route("/deleteuserconfig/<recordid>")
def delete_user_config(recordid):
	return delete_user_config_from_table(recordid)



@app.route("/result/<username>")
def get_user_file_list(username):
	path = workpath+"/result/"+username
	folder = os.path.exists(path)
	if not folder:
		return "None"
	else:
		flist = flist=os.listdir(path)
		response = {}
		serverpath = "http://hostip:port/result/"+ username +"/" 
		for fl in flist:
			response.update({fl: serverpath+fl})
	return jsonify(response)

@app.route("/result/<username>/<name>")
def get_user_file(username , name):
	path = workpath+"/result/"+username
	return send_from_directory(directory= path ,filename = name,as_attachment=True)

@app.route("/getuseralltask/<username>")
def get_user_all_task(username):
	return jsonify(get_user_task_from_table(username))



if __name__ == '__main__' :
  app.run( host = '0.0.0.0' , port = 8000 )
