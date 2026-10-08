package ws

//error codes const
const (
	RoomFull = "room-full"
	UnknownType = "unknown-type"
	InvalidMessage = "invalid-message"
)

// types const
const (
	Join = "join"
	Leave = "leave"
	Offer = "offer"
	Answer = "answer"
	Ice = "ice"
	//server messages type
	Left = "left"
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
	Payload		*WebRTCPayload	`json:"payload,omitempty"`
	Peers		[]PeerInfo		`json:"peers,omitempty"`
	Code    	string 			`json:"code,omitempty"`
	Message 	string 			`json:"message,omitempty"`
}

type WebRTCPayload struct {
	SDP           string `json:"sdp,omitempty"`
	Candidate     string `json:"candidate,omitempty"`
	SDPMid        string `json:"sdpMid,omitempty"`
	SDPMLineIndex *int   `json:"sdpMLineIndex,omitempty"` 
}

type PeerInfo struct {
	PeerId	string			`json:"peer_id"`
	Name	string			`json:"name"`
}