namespace cpp broxy

struct Request
{
	1:string url
}

struct Response
{
	1:i32 code,
	2:binary body
}

service Service {
	Response get(1:Request req)
}