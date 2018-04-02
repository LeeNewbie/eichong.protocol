package com.usrgate.config;

import java.util.concurrent.ConcurrentHashMap;

import com.netCore.model.GameObject;
public class CycleConfigs extends GameObject
{
	private static final long serialVersionUID = 8196712018915136998L;

	private ConcurrentHashMap<Integer, Integer> flowcfgs;
	private ConcurrentHashMap<Integer, Integer> instructcfgs;
	private ConcurrentHashMap<Integer, Integer> epFlowcfgs;
	private ConcurrentHashMap<Integer, Integer> epInstructcfgs;

    public CycleConfigs()
    {
    	flowcfgs = new ConcurrentHashMap<Integer, Integer>();
    	instructcfgs = new ConcurrentHashMap<Integer, Integer>();
    }

    public ConcurrentHashMap<Integer, Integer> getFlowcfgs() {
		return flowcfgs;
	}

    public void setFlowcfgs(ConcurrentHashMap<Integer, Integer> flowcfgs) {
		this.flowcfgs = flowcfgs;
	}

    public ConcurrentHashMap<Integer, Integer> getInstructcfgs() {
		return instructcfgs;
	}

    public void setInstructcfgs(ConcurrentHashMap<Integer, Integer> instructcfgs) {
		this.instructcfgs = instructcfgs;
	}

	public ConcurrentHashMap<Integer, Integer> getEpFlowcfgs() {
		return epFlowcfgs;
	}

	public void setEpFlowcfgs(ConcurrentHashMap<Integer, Integer> epFlowcfgs) {
		this.epFlowcfgs = epFlowcfgs;
	}

	public ConcurrentHashMap<Integer, Integer> getEpInstructcfgs() {
		return epInstructcfgs;
	}

	public void setEpInstructcfgs(ConcurrentHashMap<Integer, Integer> epInstructcfgs) {
		this.epInstructcfgs = epInstructcfgs;
	}
}