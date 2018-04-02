package com.usrgate.cache;

import com.usrgate.constant.BaseConstant;


public class InstructDetail {
	
	private String version;			// 版本号
	private String instructCmd;		// 指令编号
	private int instructCount;		// 指令数

	public InstructDetail(String key){
		String version, instructCmd;

		String[] val = key.split(BaseConstant.ESCAPE + BaseConstant.SPLIT);
		if (val.length == 2) {
			version = val[0];
			instructCmd = val[1];
			this.version = version;
		} else {
			instructCmd = val[0];
		}

		this.instructCmd = instructCmd;
		this.instructCount = 0;
	}
	
	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getInstructCmd() {
		return instructCmd;
	}

	public void setInstructCmd(String instructCmd) {
		this.instructCmd = instructCmd;
	}

	public int getInstructCount() {
		return instructCount;
	}

	public void setInstructCount(int instructCount) {
		this.instructCount = instructCount;
	}

	public void addInstructCount() {
		this.instructCount += 1;
	}

	public void addInstructCount(int instructCount) {
		this.instructCount += instructCount;
	}

	@Override
	public String toString() {
		
		final StringBuilder sb = new StringBuilder();
        sb.append("InstructDetail");
      
        if (version != null) sb.append(",{版本号 = ").append(version).append("}\n");
		sb.append(",{指令编号 = ").append(instructCmd).append("}\n");
		sb.append(",{指令数 = ").append(instructCount).append("}\n");
        
   		return sb.toString();
	}

}
