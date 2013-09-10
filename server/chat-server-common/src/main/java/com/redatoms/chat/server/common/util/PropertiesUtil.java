package com.redatoms.chat.server.common.util;

import java.io.InputStream;
import java.io.InterruptedIOException;
import java.lang.reflect.Field;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;

import org.apache.log4j.Logger;

public class PropertiesUtil {
	
	private static final Logger logger = Logger.getLogger(PropertiesUtil.class); 

	public static void setProperties(Object target, String configFileName) {
		if (target == null || StringUtil.isEmpty(configFileName)) {
			return;
		}
		Properties props = loadConfigFile(target, configFileName);
		if (props == null) {
			return;
		}
		Set<Entry<Object, Object>> entrySet = props.entrySet();
		for (Entry<Object, Object> entry : entrySet) {
			setProperty(target, entry);
		} 
	}
	
	public static Properties loadConfigFile(Object target, String configFileName) {
		if (target == null || StringUtil.isEmpty(configFileName)) {
			return null;
		}
		Properties props = new Properties();
		InputStream istream = null;
		try {
			istream = target.getClass().getClassLoader().getResourceAsStream(configFileName);
			props.load(istream);
			istream.close();
		} catch (Exception e) {
			if ((e instanceof InterruptedIOException)
					|| (e instanceof InterruptedException)) {
				Thread.currentThread().interrupt();
			}
			logger.error("Could not read configuration file [" + configFileName + "].", e);
			logger.error("Ignoring configuration file [" + configFileName + "].");
		} finally {
			if (istream != null) {
				try {
					istream.close();
				} catch (InterruptedIOException ignore) {
					Thread.currentThread().interrupt();
				} catch (Throwable ignore) {
				}
			}
		}
		return props;
	}
	
	public static void setProperty(Object target, Entry<Object, Object> entry) {
		try {
			Field field = target.getClass().getDeclaredField(entry.getKey().toString());
			field.setAccessible(true);
			String fieldClassName = field.getType().getName();
			logger.debug(entry.getKey().toString() + ": type = " + fieldClassName + ", value = " + entry.getValue());
			if ("java.lang.String".equals(fieldClassName)) {
				field.set(target, entry.getValue());
			} else if  ("java.lang.String".equals(fieldClassName)) {
				field.set(target, entry.getValue());
			} else if  ("java.lang.Long".equals(fieldClassName) || "long".equals(fieldClassName)) {
				field.set(target, Long.valueOf(entry.getValue().toString()));
			} else if  ("java.lang.Integer".equals(fieldClassName) || "int".equals(fieldClassName)) {
				field.set(target, Integer.valueOf(entry.getValue().toString()));
			} else if  ("java.lang.Boolean".equals(fieldClassName) || "boolean".equals(fieldClassName)) {
				field.set(target, Boolean.valueOf(entry.getValue().toString()));
			}
		} catch (Exception e) { 
			logger.error("failed to set object property: " + entry.getKey().toString(), e);
		}
		
	}
}
