package com.conqueue.client;

import com.conqueue.server.ConTask;

public abstract class AbstractObtainData {
	public abstract Object obtainData(ConTask task) throws Exception;
}
