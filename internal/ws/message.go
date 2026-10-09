package ws

import "encoding/json"

//error codes const
const (
	RoomFull = "room-full"
	UnknownType = "unknown-type"
	InvalidMessage = "invalid-message"
	PeerNotFound = "peer-not-found"
)

// types const
const (
	Offer = "offer"
	Answer = "answer"
	Ice = "ice"
	//server messages type
	Joined = "joined"
	PeerJoined = "peer-joined"
	PeerLeft = "peer-left"
	Error = "error"
)

type Message struct {
	Type		string			`json:"type"`
	RoomID		string			`json:"room_id,omitempty"`
	PeerID		string			`json:"peer_id,omitempty"`
	TargetID  	string			`json:"target_id,omitempty"`
	Name     	string			`json:"name,omitempty"`
	Payload 	json.RawMessage `json:"payload,omitempty"`
	Peers		[]PeerInfo		`json:"peers,omitempty"`
	Code    	string 			`json:"code,omitempty"`
	Message 	string 			`json:"message,omitempty"`
}

type PeerInfo struct {
	PeerId	string			`json:"peer_id"`
	Name	string			`json:"name"`
}