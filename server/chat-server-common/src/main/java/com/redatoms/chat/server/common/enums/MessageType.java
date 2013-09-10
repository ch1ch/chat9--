package com.redatoms.chat.server.common.enums;

public enum MessageType {
	CHANNEL(1),
	GROUP(2),
	CUSTOM_GROUP(3),
	PERSON_TO_PERSON(4);
	
	private int value;

	MessageType(int value) {
		this.value = value;
	}

	public int value() {
		return value;
	}

	public static MessageType getEnum(int value) {
		switch (value) {
		case 1:
			return CHANNEL;
		case 2:
			return GROUP;
		case 3:
			return CUSTOM_GROUP;
		case 4:
			return PERSON_TO_PERSON;
		default:
			return null;
		}
	}
}
