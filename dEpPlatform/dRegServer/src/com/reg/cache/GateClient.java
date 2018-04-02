package com.reg.cache;


public class GateClient  extends NetObject {
	
	private int type;
	
	private int id;
	private String ip;
	
	public GateClient()
	{
	}
	
	
	
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	
	
	

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	
	
	@Override
	public String toString() {
		
		final StringBuilder sb = new StringBuilder();
        sb.append("GateClient");
        
        sb.append("{type=").append(type).append("}\n");
        sb.append(",{id=").append(id).append("}\n");
        
        sb.append(",{ip=").append(ip).append("}\n");
   		return sb.toString();
	}

	
}
