
import com.conqueue.client.ObtainData;
import com.conqueue.server.ConTask;

public class QueMain {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {

		//RecTask.setThreadMaxNum(4);
		for (int i = 0; i < 1; i++) {
			ConTask task = new ConTask();
			task.taskID = i;
			//ObtainData ob = new ObtainData();
			//ob.obtainData(task);
			task.start();
		}
	}

}
