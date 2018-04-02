package com.usrgate.cache;


public class InstructStat {
	
	private InstructStatistics instructMax;
	private InstructStatistics instructTotal;

	public InstructStat(int cycle){
		this.instructMax = new InstructStatistics(cycle);
		this.instructTotal = new InstructStatistics(cycle);
	}

	public InstructStatistics getInstructMax() {
		return instructMax;
	}

	public void setInstructMax(InstructStatistics instructMax) {
		if (instructMax.getInstructCount() > this.instructMax.getInstructCount()) {
			this.instructMax.setInstructMap(instructMax.getInstructMap());
			this.instructMax.setInstructCount(instructMax.getInstructCount());
			this.instructMax.setCycle(instructMax.getCycle());
			this.instructMax.setUserMarker(instructMax.getUserMarker());
			this.instructMax.setStatisticsTime(instructMax.getStatisticsTime());
		}
	}

	public InstructStatistics getInstructTotal() {
		return instructTotal;
	}

	public void setInstructTotal(InstructStatistics instructTotal) {
		if (instructTotal.getInstructCount() > this.instructTotal.getInstructCount()) {
			this.instructTotal.setInstructMap(instructTotal.getInstructMap());
			this.instructTotal.setInstructCount(instructTotal.getInstructCount());
			this.instructTotal.setCycle(instructTotal.getCycle());
			this.instructTotal.setStatisticsTime(instructTotal.getStatisticsTime());
		}
	}

	@Override
	public String toString() {
		
		StringBuilder sb = new StringBuilder();
		
        sb.append("单个通道的新高指令数:").append("\n");
		sb.append(instructMax).append("\n");	
		
        sb.append("所有通道的新高指令数:").append("\n");
		sb.append(instructTotal).append("\n");	
        
   		return sb.toString();
	} 

}
