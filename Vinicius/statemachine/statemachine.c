
int GetEvent();
int GetAction(int state, int event);
int DoAction(int action);
int NewState(int state, int event);


task main()
{
	int state, event, action;
	while(true){
		if(event==NULL) event=getEvent();
		action=GetAction(state, event);
		event=DoAction(action);
		state=NewState(state, event);
	}
}
