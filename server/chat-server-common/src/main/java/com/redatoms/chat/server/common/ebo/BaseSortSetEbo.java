package com.redatoms.chat.server.common.ebo;

public abstract class BaseSortSetEbo<T> extends BaseEbo<T> {

	private static final long serialVersionUID = 1L;
	
	private long version;

	public long getVersion() {
		return version;
	}

	public void setVersion(long version) {
		this.version = version;
	}

	
}
