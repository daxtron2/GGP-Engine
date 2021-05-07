
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

VertexToPixel main( uint vertexId : SV_VertexID )
{
	/* The screen's dimensions are [-1, +1], [-1, +1].
	*  
	*  Create a triangle with vertex coordinates (-1, +1), (+3, +1), (-1, -3).
	*
	*  The UV can then be placed on this triangle with UV:(0, 0) -> pos:(-1, +1),
	*  UV:(1, 0) -> pos:(+1, +1), UV:(1, 1) -> pos:(+1, -1), and UV:(0, 1) -> pos:(-1, -1).
	* 
	*  To line up these UV -> pos values, use UV:(0, 0) -> pos:(-1, +1), UV:(2, 0) -> pos:(+3, +1), and UV:(0, 2) -> pos:(-1, -3).
	*  UV wrapping will not matter since the screen will only render within +/-1 for X/Y.
	*/

	// Given an input vertex ID, generate a corresponding vertex matching the above description
	VertexToPixel output;

	output.uv = float2(0, 0);
	output.uv.x = (vertexId << 1) & 2; // Add 2 if the first bit is 1 (ID #1)
	output.uv.y = (vertexId & 2); // Add 2 if the second bit is 1 (ID #2)

	output.position = float4(0, 0, 0, 1);
	output.position.x = (output.uv.x) * 2 - 1;
	output.position.y = (output.uv.y) * -2 + 1;

	return output;
}