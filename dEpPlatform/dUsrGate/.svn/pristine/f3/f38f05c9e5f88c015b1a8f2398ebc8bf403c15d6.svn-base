package com.usrgate.service;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.usrgate.cache.ProtocolVersion;

public class VersionService {
	
	private static Map<Integer, ProtocolVersion> versionMap = new ConcurrentHashMap<Integer, ProtocolVersion>();

	public static Map<Integer, ProtocolVersion> getMapVersion()
	{
		return versionMap;
	}
	
	public static ProtocolVersion getVersion(int version)
	{
		return versionMap.get(version);
	}
	
	public static void putVersion(ProtocolVersion protocolVersion)
	{
		versionMap.put(protocolVersion.getVersion(), protocolVersion);
	}

	public static void addVersion(int version, int accountId)
	{
		ProtocolVersion protocolVersion = getVersion(version);
		if (protocolVersion == null) {
			protocolVersion = new ProtocolVersion(version, accountId);
		} else {
			protocolVersion.addCount(accountId);
		}
		putVersion(protocolVersion);
	}

    public static void offProtocol(int version)
	{
		ProtocolVersion userVersion = getVersion(version);
		if (userVersion != null) {
			userVersion.minusCount();
			if (userVersion.getCount() == 0) versionMap.remove(version);
		}
	}
}
