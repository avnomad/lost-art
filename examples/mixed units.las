<?xml version="1.0" encoding="utf-8"?>
<!--
	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>

	This file is part of LostArt.

	LostArt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LostArt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
-->

<!--
	Two controls of fixed size have fixed positions relative
	to the window and each other.

	Some of the distances are expressed in pixels, while
	others in millimeters.
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>10</left>
				<bottom>10</bottom>
				<right>272.0833</right>
				<top>247.2222</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>39.5698433</left>
				<bottom>225.987518</bottom>
				<right>105.859421</right>
				<top>176.598633</top>
			</sides>
			<borderSize>2</borderSize>
			<name>control0</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>163.677521</left>
				<bottom>160.566071</bottom>
				<right>224.325424</right>
				<top>103.416092</top>
			</sides>
			<borderSize>2</borderSize>
			<name>control1</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>30mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>0</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>198.043884</first-local-side>
			<second-local-side>205.043884</second-local-side>
		</constraint>
		<constraint>
			<text>100px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>169.332199</first-local-side>
			<second-local-side>176.332214</second-local-side>
		</constraint>
		<constraint>
			<text>50px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>106.137581</first-local-side>
			<second-local-side>113.137451</second-local-side>
		</constraint>
		<constraint>
			<text>100mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>96.0902939</first-local-side>
			<second-local-side>103.090294</second-local-side>
		</constraint>
		<constraint>
			<text>50mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>224.550949</first-local-side>
			<second-local-side>231.550949</second-local-side>
		</constraint>
		<constraint>
			<text>20mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>69.6068344</first-local-side>
			<second-local-side>76.6068497</second-local-side>
		</constraint>
		<constraint>
			<text>20px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>190.91658</first-local-side>
			<second-local-side>197.91658</second-local-side>
		</constraint>
		<constraint>
			<text>30px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>135.857086</first-local-side>
			<second-local-side>142.857086</second-local-side>
		</constraint>
	</constraints>
</gui-model>
